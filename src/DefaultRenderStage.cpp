/** @file DefaultRenderStage.cpp



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

#include "ugl/DefaultRenderStage.hpp"

#include <iostream>

namespace ugl
{

// ------------------------------------------------------------------------

void DefaultRenderStage::addDrawable( Drawable* sr )
{
    m_drawables.push_back( sr );
}


/**
 * @brief Removes given drawable if it exists.
 * @param drawable
 */
void DefaultRenderStage::removeDrawable(Drawable* drawable)
{
    if (std::find(m_drawables.begin(), m_drawables.end(), drawable)!=m_drawables.end())
      m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), drawable),m_drawables.end());
}

// ------------------------------------------------------------------------

DefaultRenderStage::DefaultRenderStage()
{
    //background
    m_bgProgram.addImportPath("shader");
    m_bgProgram.addShaderFromSourceFile(ugl::COMBINED, "ugl/background.glsl");
    glGenVertexArrays(1, &m_bgVAO);
}

// ------------------------------------------------------------------------

void DefaultRenderStage::draw( StateSet& state, bool drawBg )
{
    m_stateSet.setParent( state );

    const glm::uvec4& viewport = *(state.getViewport());

    glViewport( viewport[0], viewport[1], viewport[2], viewport[3] );

    // clear framebuffer
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClearDepth( 1.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //draw background if wanted
    UniformSet& uniforms = state.getOrCreateUniforms();
    uniforms.set("drawBg", drawBg);
    m_stateSet.apply(m_bgProgram);
    glBindVertexArray(m_bgVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    for( std::vector<Drawable*>::iterator di=m_drawables.begin(); di != m_drawables.end(); ++di )
        if ((*di)->isVisible())
            (*di)->draw( m_stateSet );          
}
    
} // namespace ugl
