/** @file TransparentRenderStageDFB.hpp



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

#ifndef TRANSPARENTRENDERSTAGEDFB_HPP
#define TRANSPARENTRENDERSTAGEDFB_HPP

#include <GL/glew.h>

#include "Drawable.hpp"
#include "Volume.hpp"
#include "ShaderProgram.hpp"
#include "StateSet.hpp"
#include "Framebuffer.hpp"

#include <vector>

namespace ugl
{

/**
 * Renders its \ref Drawable "Drawables" using a Dynamic Fragment Buffer for Order Independent Transparency.
 */
class TransparentRenderStageDFB
{
public:
    TransparentRenderStageDFB();
    virtual ~TransparentRenderStageDFB();

    void draw(const StateSet& state);
    Framebuffer& drawOffscreen(const StateSet& state);

    void addDrawable(Drawable* drawable);
    void removeDrawable(Drawable* drawable);
    const std::vector<Drawable*>& getDrawables() const;

    void addVolume(Volume* volume);

    void setCheckerboard(bool showCheckerboard);
    bool getCheckerboard();

    void setBackgroundColor(const glm::vec4 &color);
    glm::vec4 getBackgroundColor();

protected:
    bool resize(unsigned int width, unsigned int height);

protected:    
    unsigned int width;
    unsigned int height;

	StateSet stateSet;
	std::vector<Drawable*> drawables;

    VariantProgram clearProgram;
    VariantProgram prefixSumProgram;
    VariantProgram displayProgram;
    
    GLuint countingBuffer;
    GLuint offsetBuffer;
    GLuint fragmentBuffer;
    GLuint numFragmentsBuffer;

	GLuint fullScreenQuadVAO;

    glm::vec4 backgroundColor;
    bool checkerboard;

    Framebuffer framebuffer;
};

}
#endif
