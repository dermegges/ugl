/** @file MeshDrawable.cpp



Copyright 2016 Computational Topology Group, University of Kaiserslautern

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

    Author(s): C.Garth, T.Biedert
*/

#include <vector>

#include "ugl/GLHelper.hpp"
#include "ugl/MeshDrawable.hpp"

namespace ugl
{

MeshDrawable::MeshDrawable( MeshData& data) :
    m_boundingBox( data.getBoundingBox() ),
    m_drawSurface( true ),
    m_drawEdges( false ),
    m_alpha( 1.0f )
{
    prepareShaderProgram();
    prepareVertexArrays( data );
}




/**
 * @brief Returns the addresses of the parameters which should be GUI controllable.
 * @param drawSurface
 * @param drawEdges
 * @param alpha
 */
void MeshDrawable::getTweakableParameters(bool** drawSurface, bool** drawEdges, float** alpha)
{
    *drawSurface = &this->m_drawSurface;
    *drawEdges = &this->m_drawEdges;
    *alpha = &this->m_alpha;
}

// -------------------------------------------------------------------------

void MeshDrawable::prepareShaderProgram()
{
    m_program.addImportPath("shader");

    // add shader
    m_program.addShaderFromSourceFile( COMBINED, "ugl/mesh.glsl" );

    m_program.addAttributeLocation( "vertexPosition", 0u );
    m_program.addAttributeLocation( "vertexNormal",   1u );
}

// -------------------------------------------------------------------------

void MeshDrawable::prepareVertexArrays( MeshData& data )
{
    // set up the vertex arrays and attribute bindings
    glGenVertexArrays( 1, &m_vertexArray );
    glBindVertexArray( m_vertexArray );

    // prepare vertex position buffer
    m_vertexPositionBuffer = GLHelper::prepareStaticBuffer( data.getPoints() );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexPositionBuffer );
    glVertexAttribPointer( 0u, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    glEnableVertexAttribArray( 0u );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );

    // prepare vertex normal buffer
    m_vertexNormalBuffer = GLHelper::prepareStaticBuffer( data.getVertexNormals() );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexNormalBuffer );
    glVertexAttribPointer( 1u, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    glEnableVertexAttribArray( 1u );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );

    // prepare triangle index buffer
    m_triangleBuffer    = GLHelper::prepareStaticBuffer( data.getTriangles() );
    m_triangleCount     = data.getTriangleCount() * 3;

    // prepare edge index buffer
    m_edgeBuffer        = GLHelper::prepareStaticBuffer( data.getEdges() );
    m_edgeCount         = data.getEdgeCount() * 2;

    // NOTE: we do not bind an ELEMENT_ARRAY here, since we will switch
    // on the fly between triangle and edge indices depending on render mode

    glBindVertexArray( 0u );
}

// -------------------------------------------------------------------------

void MeshDrawable::draw( const StateSet& state )
{
    m_stateSet.setParent(state);
    UniformSet& uniforms = m_stateSet.getOrCreateUniforms();
    ModeSet&    modes    = m_stateSet.getOrCreateModes();

    uniforms.set( "alpha", m_alpha );

    // set render modes and issue draw calls
    glBindVertexArray( m_vertexArray );

    if( m_drawSurface )
    {
        modes.clear( "LINE_MODE" );

        // bind program and add uniforms
        m_stateSet.apply( m_program );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_triangleBuffer );
        glDrawElements( GL_TRIANGLES, m_triangleCount, GL_UNSIGNED_INT,
                        nullptr );
    }

    if( m_drawEdges )
    {
        modes.set( "LINE_MODE", 1 );

        // bind program and add uniforms
        m_stateSet.apply( m_program );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_edgeBuffer );
        glDrawElements( GL_LINES, m_edgeCount, GL_UNSIGNED_INT, nullptr );
    }

    glBindVertexArray( 0u );
    // done drawing
}

} // namespace ugl
