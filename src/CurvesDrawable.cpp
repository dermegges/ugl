/** @file CurvesDrawable.cpp



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

#include "ugl/CurvesDrawable.hpp"
#include "ugl/GLHelper.hpp"
#include "ugl/NoValues.hpp"

namespace ugl
{

CurvesDrawable::CurvesDrawable( CurvesData& data ) :
    // prepare start index array
    m_startIndices( data.getAllStartPointIds() ),
    // prepare index count array
    m_indexCounts( data.getAllPointCounts() ),
    m_values( new NoValues() ),
    m_boundingBox( data.getBoundingBox() ),
    m_colorCurves( true ),
    m_alpha( 0.2f )
{
    prepareShaderProgram();
    prepareVertexArrays( data );
}

// -------------------------------------------------------------------------

void CurvesDrawable::prepareShaderProgram()
{
    m_program.addImportPath( "shader" );

    // add shader
    m_program.addShaderFromSourceFile( COMBINED, "ugl/curves.glsl" );

    m_program.addAttributeLocation( "vertexPosition", 0u );
    m_program.addAttributeLocation( "vertexCurveId", 1u );
}

// -------------------------------------------------------------------------

void CurvesDrawable::prepareVertexArrays( CurvesData& data )
{
    // set up the vertex arrays and attribute bindings
    glGenVertexArrays( 1, &m_vertexArray );
    glBindVertexArray( m_vertexArray );

    // prepare vertex position buffer
    m_vertexPositionBuffer = GLHelper::prepareStaticBuffer( data.getAllPoints() );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexPositionBuffer );
    glVertexAttribPointer( 0u, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    glEnableVertexAttribArray( 0u );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );

    // prepare vertex curve buffer
    m_vertexCurveIdBuffer = GLHelper::prepareStaticBuffer(
                              data.getAllCurveIdsOfPointIds() );
    glBindBuffer( GL_ARRAY_BUFFER, m_vertexCurveIdBuffer );
    glVertexAttribIPointer( 1u, 1, GL_INT, 0, nullptr );
    glEnableVertexAttribArray( 1u );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );

    // prepare buffer texture for curve colors
    {
        std::vector<glm::vec3> startPoints;
        startPoints.reserve( data.getCurveCount() );
        for( GLint curveId = 0; curveId < data.getCurveCount(); ++curveId )
            startPoints.push_back( data.getPoint( curveId, 0 ) );

        BoundingBox startPointsBox;
        startPointsBox.computeFromPositions( startPoints );

        for( std::vector<glm::vec3>::iterator point = startPoints.begin(); point != startPoints.end(); ++point )
            *point = startPointsBox.normalizePosition( *point );

        m_curveColorBuffer = GLHelper::prepareStaticBuffer( startPoints );
        glGenTextures( 1, &m_curveColorTex );
        glBindTexture( GL_TEXTURE_BUFFER, m_curveColorTex );
        glTexBuffer( GL_TEXTURE_BUFFER, GL_RGB32F, m_curveColorBuffer );
        m_stateSet.getOrCreateUniforms().setTexture( "curveColors",
                GL_TEXTURE_BUFFER, m_curveColorTex );
        glBindTexture( GL_TEXTURE_BUFFER, 0u );
    }

    glBindVertexArray( 0u );
}

// -------------------------------------------------------------------------

void CurvesDrawable::addValues(Values* values)
{
    delete m_values;
    m_values = values;

    glBindVertexArray( m_vertexArray );
    m_values->initialize( m_stateSet, m_program );
    glBindVertexArray( 0u );    
}

// -------------------------------------------------------------------------


/**
 * @brief Returns the addresses of the GUI-controllable parameters.
 * @param colorCurves
 * @param alpha
 */
void CurvesDrawable::getTweakableParameters(bool** colorCurves, float** alpha)
{
    *colorCurves = &this->m_colorCurves;
    *alpha = &this->m_alpha;
}


// -------------------------------------------------------------------------

void CurvesDrawable::draw( const StateSet& state )
{
    m_stateSet.setParent( state );
    UniformSet& uniforms = m_stateSet.getOrCreateUniforms();
    ModeSet&    modes    = m_stateSet.getOrCreateModes();

    // set render modes and issue draw calls
    glBindVertexArray( m_vertexArray );

    modes.setOrClear( "COLOR_CURVES", m_colorCurves );
    uniforms.set( "alpha", m_alpha );

    m_values->extendDraw( m_stateSet );

    // bind program and add uniforms
    m_stateSet.apply( m_program );

    glMultiDrawArrays( GL_LINE_STRIP, m_startIndices.data(),
                       m_indexCounts.data(), m_startIndices.size() );

    glBindVertexArray( 0u );
    // done drawing
}

} // namespace ugl
