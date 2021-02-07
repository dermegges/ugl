/** @file DefaultRenderStage.hpp



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

#ifndef __DefaultRenderStage_hpp
#define __DefaultRenderStage_hpp

#include <GL/glew.h>

#include "Drawable.hpp"
#include "ShaderProgram.hpp"
#include "StateSet.hpp"

#include <vector>

namespace ugl
{

/**
 * Renders its \ref Drawable "Drawables" straight into the
 * framebuffer.
 */
class DefaultRenderStage
{
public:

    DefaultRenderStage();

    void resize( unsigned int width, unsigned int height );

    void draw( StateSet& state, bool drawBg = false );

    void addDrawable( Drawable* sr );

	  void removeDrawable(Drawable* drawable);

    int getNumDrawables();

    int getNumVisibleDrawables();

protected:

    std::vector<Drawable*>  m_drawables;
    StateSet                m_stateSet;

    //background
    GLuint m_bgVAO;
    VariantProgram m_bgProgram;
};

inline int DefaultRenderStage::getNumDrawables()
{
    return m_drawables.size();
}

inline int DefaultRenderStage::getNumVisibleDrawables()
{
    int numVisD = 0;
    for( std::vector<Drawable*>::iterator di=m_drawables.begin(); di != m_drawables.end(); ++di )
        if ((*di)->isVisible())
            numVisD++;    
    return numVisD;
}

} // namespace ugl

#endif // __DefaultRenderStage_hpp
