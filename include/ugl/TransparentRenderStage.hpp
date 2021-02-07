/** @file TransparentRenderStage.hpp



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

#ifndef __TransparentRenderStage_hpp
#define __TransparentRenderStage_hpp

#include <GL/glew.h>

#include "Drawable.hpp"
#include "Volume.hpp"
#include "ShaderProgram.hpp"
#include "StateSet.hpp"
#include "Framebuffer.hpp"
#include "MeshData.hpp"
#include "MeshDrawable.hpp"

#include <vector>

namespace ugl
{

/**
 * Renders its \ref Drawable "Drawables" using Depth Peeling for Order
 * Independent Transparency.
 */
class TransparentRenderStage
{
public:
    TransparentRenderStage(unsigned int numSamples = 0, unsigned int maxPasses = 20, unsigned int samplesThreshold = 100);

    void resize(unsigned int width, unsigned int height);

    void draw(const StateSet& state);
    Framebuffer& drawOffscreen(const StateSet& state);

    void addDrawable(Drawable* drawable);
    void removeDrawable(Drawable* drawable);
    
    void addVolume(Volume* volume);

    unsigned int getNumSamples();
    void setNumSamples(unsigned int numSamples);

    unsigned int getMaxPasses();
    void setMaxPasses(unsigned int maxPasses);

    void setCheckerboard(bool showCheckerboard);
    bool getCheckerboard();

    void setBackgroundColor(const glm::vec4 &color);
    glm::vec4 getBackgroundColor();

    const std::vector<Drawable*>* getDrawables();

protected:    
    GLuint width;
    GLuint height;

    Framebuffer peelBuffers[2];
    Framebuffer targetBuffers[2];

    GLenum textureTarget;

    GLuint fullScreenQuadVAO;

    std::vector<Drawable*> drawables;
    Volume* volume;

    VariantProgram blendProgram;
    StateSet stateSet;

    unsigned int numSamples;
    unsigned int maxPasses;
    unsigned int samplesThreshold;

    glm::vec4 backgroundColor;
    bool checkerboard;
};

}
#endif
