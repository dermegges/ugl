#include <set>

#include <GL/glew.h>

#include "SurfaceRenderer.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// -------------------------------------------------------------------------

SurfaceRenderer::SurfaceRenderer( const SurfaceVectorData& svd )
{
    prepareShaderProgram();
    prepareVertexArrays( svd );

    // --- set default parameters

    m_alpha           = 0.3f;
    m_showVectors     = false;
    m_drawEdges       = false;
    m_drawSurface     = true;
    m_useVertexNormal = true;

    pair<glm::vec3, glm::vec3> box = computeBoundingBox( svd );
    vector<glm::vec3> boxPositions;
    boxPositions.push_back( box.first );
    boxPositions.push_back( box.second );
    m_boundingBox.computeFromPositions( boxPositions );

    // --- set up TweakBar

    TwBar* bar = TwNewBar( "Surface View" );

    TwAddVarRW( bar, "edges",   TW_TYPE_BOOLCPP, &m_drawEdges,       "label='Draw Edges' key=e" );
    TwAddVarRW( bar, "surface", TW_TYPE_BOOLCPP, &m_drawSurface,     "label='Draw Surface' key=s" );
    TwAddVarRW( bar, "vnorm",   TW_TYPE_BOOLCPP, &m_useVertexNormal, "label='Use Vertex Normals' key=n" );
    TwAddVarRW( bar, "vectors", TW_TYPE_BOOLCPP, &m_showVectors,     "label='Show Vectors' key=v" );
    TwAddVarRW( bar, "alpha",   TW_TYPE_FLOAT,   &m_alpha,           "label='Alpha' min=0 max=1 step=0.01" );
}

// -------------------------------------------------------------------------

void SurfaceRenderer::prepareShaderProgram()
{
    // --- set up shader program
    m_program.addImportPath( "shader" );

    // add shaders
    m_program.addShaderFromSourceFile( ugl::COMBINED, "surface.glsl" );

    m_program.addAttributeLocation( "vertexPosition", 0u );
    m_program.addAttributeLocation( "vertexVector", 1u );
    m_program.addAttributeLocation( "vertexNormal", 2u );
}

// -------------------------------------------------------------------------

/// helper function for repeated buffer creation
template<typename T>
GLuint prepareStaticBuffer( const std::vector<T>& data )
{
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(T)*data.size(), &data[0], GL_STATIC_DRAW );

    return buffer;
}

// -------------------------------------------------------------------------

void SurfaceRenderer::prepareVertexArrays( const SurfaceVectorData& svd )
{
    // prepare vertex positions buffer
    {
        vector<glm::vec3> vertexData( svd.getNumberOfPoints() );

        for( unsigned int i=0; i<svd.getNumberOfPoints(); ++i )
            vertexData[i] = svd.getPoint(i);

        m_vertexBuffer = prepareStaticBuffer( vertexData );
    }

    // prepare vertex normals buffer
    {
        vector<glm::vec3> normalData = computeVertexNormals( svd );
        m_vertexNormalBuffer = prepareStaticBuffer( normalData );
    }

    // prepare face normals buffer
    {
        vector<glm::vec3> normalData = computeFaceNormals( svd );
        m_faceNormalBuffer = prepareStaticBuffer( normalData );
    }

    // prepare vertex vectors buffer
    {
        // for each vector, find the length and create an array
        // (also compute max vector length and normalize to [0,1])
        float maxLength = 0.0;

        vector<glm::vec3> vectorData( svd.getNumberOfPoints() );

        for( unsigned int i=0; i<svd.getNumberOfPoints(); ++i )
        {
            vectorData[i] = svd.getVector( i );
            maxLength = std::max( glm::length(vectorData[i]), maxLength );
        }

        // normalize vectors' length to [0,1]
        for( unsigned int i=0; i<vectorData.size(); ++i )
            vectorData[i] /= maxLength;

        m_vectorBuffer = prepareStaticBuffer( vectorData );
    }

    // prepare triangle index buffer
    {
        vector<glm::uvec3> triData( svd.getNumberOfTriangles() );

        for( unsigned int i=0; i<triData.size(); ++i )
            triData[i] = svd.getTriangleIndices(i);

        m_triIndexBuffer = prepareStaticBuffer( triData );
        m_triIndexSize   = triData.size() * 3;
    }

    // prepare edge index buffer
    {
        vector<glm::uvec2> edgeData = computeEdges( svd );

        m_edgeIndexBuffer = prepareStaticBuffer( edgeData );
        m_edgeIndexSize   = edgeData.size() * 2;
    }

    // set up the vertex array and attribute bindings
    glGenVertexArrays( 1, &m_vertexArray );
    glBindVertexArray( m_vertexArray );

    glBindBuffer( GL_ARRAY_BUFFER, m_vertexBuffer );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, m_vectorBuffer );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, m_vertexNormalBuffer );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, NULL );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );

    // NOTE: we do not bind an ELEMENT_ARRAY here, since we will switch
    // on the fly between triangle and edge indices depending on render mode

    // set up buffer texture for face normals
    glGenTextures( 1, &m_faceNormalTex );
    glBindTexture( GL_TEXTURE_BUFFER, m_faceNormalTex );
    glTexBuffer( GL_TEXTURE_BUFFER, GL_RGB32F, m_faceNormalBuffer );
    // assign buffer texture
    m_stateSet.getOrCreateUniforms().setTexture( "faceNormal",
            GL_TEXTURE_BUFFER, m_faceNormalTex );
}

// ------------------------------------------------------------------------

void SurfaceRenderer::draw( const ugl::StateSet &state )
{
    m_stateSet.setParent(state);
    ugl::UniformSet& uniforms = m_stateSet.getOrCreateUniforms();
    ugl::ModeSet&    modes    = m_stateSet.getOrCreateModes();

    uniforms.set( "alpha", m_alpha );

    modes.setOrClear( "USE_VERTEX_NORMAL", m_useVertexNormal );
    modes.setOrClear( "SHOW_VECTORS",      m_showVectors );

    // set render modes and issue draw calls
    glBindVertexArray( m_vertexArray );

    if( m_drawSurface )
    {
        modes.clear( "LINE_MODE" );

        // bind program and add uniforms
        m_stateSet.apply(m_program);

        glPolygonOffset( 1.0, 2.0 );
        glEnable( GL_POLYGON_OFFSET_FILL );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_triIndexBuffer );
        glDrawElements( GL_TRIANGLES, m_triIndexSize, GL_UNSIGNED_INT, (GLvoid*)0 );

        glDisable( GL_POLYGON_OFFSET_FILL );
    }

    if( m_drawEdges )
    {
        modes.set( "LINE_MODE", 1 );

        // bind program and add uniforms
        m_stateSet.apply(m_program);

        if( modes.exists( "DEPTH_PEELING" ) )
        {
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            glDepthMask( GL_FALSE );
            glEnable( GL_BLEND );
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_edgeIndexBuffer );
        glDrawElements( GL_LINES, m_edgeIndexSize, GL_UNSIGNED_INT, (GLvoid*)0 );

        if( modes.exists( "DEPTH_PEELING" ) )
        {
            glDepthMask( GL_TRUE );
            glDisable( GL_BLEND );
        }
    }

    glBindVertexArray( 0 );
    // done drawing
}

// -------------------------------------------------------------------------

vector<glm::uvec2>
SurfaceRenderer::computeEdges( const SurfaceVectorData& svd )
{
    using std::min;
    using std::max;

    // first, count the edges emanating from each vertex
    // (to avoid counting opposite edges twice, an edge is always taken
    // to emanate from the vertex with the smaller index)
    typedef set< pair<unsigned int, unsigned int> > edge_set;

    edge_set edges;

    for( unsigned int i=0; i<svd.getNumberOfTriangles(); ++i )
    {
        glm::uvec3 tind = svd.getTriangleIndices( i );

        // 1st edge: 0 -> 1
        edges.insert( make_pair( min(tind[0],tind[1]), max(tind[0],tind[1]) ) );
        // 2nd edge: 1 -> 2
        edges.insert( make_pair( min(tind[1],tind[2]), max(tind[1],tind[2]) ) );
        // 3rd edge: 2 -> 0
        edges.insert( make_pair( min(tind[2],tind[0]), max(tind[2],tind[0]) ) );
    }

    // copy edges from the set to a linear array
    vector<glm::uvec2> output( edges.size() );

    unsigned int cnt = 0;

    for( edge_set::const_iterator si=edges.begin(); si!=edges.end(); ++si, ++cnt )
        output[cnt] = glm::uvec2(si->first, si->second);

    return output;
}

// -------------------------------------------------------------------------

vector<glm::vec3>
SurfaceRenderer::computeFaceNormals( const SurfaceVectorData& svd )
{
    vector<glm::dvec3> normals( svd.getNumberOfTriangles(), glm::dvec3(0,0,0) );

    for( unsigned int i=0; i<svd.getNumberOfTriangles(); ++i )
    {
        glm::uvec3 tri = svd.getTriangleIndices( i );

        glm::dvec3 p0 = glm::dvec3( svd.getPoint( tri[0] ) );
        glm::dvec3 p1 = glm::dvec3( svd.getPoint( tri[1] ) );
        glm::dvec3 p2 = glm::dvec3( svd.getPoint( tri[2] ) );

        glm::dvec3 n = glm::cross( p1-p0, p2-p0 );

        if( glm::length(n) > 0 )
            n = glm::normalize( n );

        normals[i] = n;
    }

    return vector<glm::vec3>( normals.begin(), normals.end() );
}

// -------------------------------------------------------------------------

vector<glm::vec3>
SurfaceRenderer::computeVertexNormals( const SurfaceVectorData& svd )
{
    vector<glm::dvec3> normals( svd.getNumberOfPoints(), glm::dvec3(0,0,0) );

    for( unsigned int i=0; i<svd.getNumberOfTriangles(); ++i )
    {
        glm::uvec3 tri = svd.getTriangleIndices( i );

        glm::dvec3 p0 = glm::dvec3( svd.getPoint( tri[0] ) );
        glm::dvec3 p1 = glm::dvec3( svd.getPoint( tri[1] ) );
        glm::dvec3 p2 = glm::dvec3( svd.getPoint( tri[2] ) );

        glm::dvec3 n = glm::cross( p1-p0, p2-p0 );

        normals[tri[0]] += n;
        normals[tri[1]] += n;
        normals[tri[2]] += n;
    }

    for( unsigned int i=0; i<normals.size(); ++i )
        if( glm::length(normals[i]) > 0 )
            normals[i] = glm::normalize( normals[i] );

    return vector<glm::vec3>( normals.begin(), normals.end() );
}

// -------------------------------------------------------------------------

pair<glm::vec3,glm::vec3>
SurfaceRenderer::computeBoundingBox( const SurfaceVectorData& svd )
{
    if( svd.getNumberOfPoints() == 0 )
        return make_pair( glm::vec3(-1,-1,-1), glm::vec3(1,1,1) );

    glm::dvec3 bmin = svd.getPoint( 0 );
    glm::dvec3 bmax = svd.getPoint( 0 );

    for( unsigned int i=1; i<svd.getNumberOfPoints(); ++i )
    {
        bmin = glm::min( bmin, svd.getPoint(i) );
        bmax = glm::max( bmax, svd.getPoint(i) );
    }

    return make_pair( glm::vec3(bmin), glm::vec3(bmax) );
}
