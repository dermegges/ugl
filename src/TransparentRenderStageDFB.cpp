/** @file TransparentRenderStageDFB.cpp



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

#include "ugl/TransparentRenderStageDFB.hpp"
#include "ugl/ErrorCheck.hpp"

#include <iostream>
#include <sstream>

#include <chrono>

namespace ugl
{


/**
 * @brief Constructor.
 */
TransparentRenderStageDFB::TransparentRenderStageDFB() : width(0), height(0)
{
    // Initialize background
    this->setBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    this->setCheckerboard(true);

    // Initialize counting buffer
    glGenTextures(1, &this->countingBuffer);

    // Initialize offset buffer
    glGenTextures(1, &this->offsetBuffer);

    // Initialize fragment buffer
    glGenBuffers(1, &this->fragmentBuffer);

    glGenBuffers(1, &this->numFragmentsBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->numFragmentsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(uint), 0, GL_DYNAMIC_READ);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Initialize full screen quad
    glGenVertexArrays(1, &this->fullScreenQuadVAO);

    // Initialize clear program
    this->clearProgram.addImportPath("shader");
    this->clearProgram.addShaderFromSourceFile( COMBINED, "ugl/dfbClear.glsl" );

    // Initialize prefix sum compute program
    this->prefixSumProgram.addImportPath("shader");
    this->prefixSumProgram.addShaderFromSourceFile( COMPUTE, "ugl/dfbPrefixSum.glsl" );

    // Initialize display program
    this->displayProgram.addImportPath("shader");
    this->displayProgram.addShaderFromSourceFile( COMBINED, "ugl/dfbDisplay.glsl" );
}


/**
 * @brief Destructor.
 */
TransparentRenderStageDFB::~TransparentRenderStageDFB()
{
    glDeleteVertexArrays(1, &this->fullScreenQuadVAO);
    glDeleteBuffers(1, &this->fragmentBuffer);
    glDeleteBuffers(1, &this->numFragmentsBuffer);
    glDeleteTextures(1, &this->offsetBuffer);
    glDeleteTextures(1, &this->countingBuffer);
}



/**
 * @brief Draws everything using depth peeling and automatically resolves the target buffer.
 * @param state
 */
void TransparentRenderStageDFB::draw(const StateSet& state)
{
    // Frame time benchmark
//    std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

//    int n = 1000;
//    for (int i = 0; i < n; ++i)
//        this->drawOffscreen(state);

//    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
//    std::cout << "Frame time: " << (ms / n) << " ms" << std::endl;

    this->drawOffscreen(state).resolve();
}



/**
 * @brief Draws everything into the offscreen dynamic fragment buffer.
 * @param state
 */
Framebuffer& TransparentRenderStageDFB::drawOffscreen(const StateSet& state)
{
    this->stateSet.setParent(state);
    UniformSet& uniforms = this->stateSet.getOrCreateUniforms();
    ModeSet& modes = this->stateSet.getOrCreateModes();

    modes.set("DYNAMIC_FRAGMENT_BUFFER", 1);


    glDisable(GL_DEPTH_TEST);


    // Resize buffers if necessary
    const glm::uvec4& viewport = *(state.getViewport());
    const unsigned int width  = viewport[2];
    const unsigned int height = viewport[3];

    bool resized = this->resize(width, height);

    /*
     * Clear counting buffer (only at first frame after resize).
     */
    if (resized)
    {
        uniforms.setImageTexture(0, this->countingBuffer, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32UI);

        this->stateSet.apply(this->clearProgram);

        glBindVertexArray(this->fullScreenQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    /*
     * Render all drawables to count per-pixel and total fragment counts.
     */
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    uniforms.setImageTexture(0, this->countingBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    modes.set("DYNAMIC_FRAGMENT_BUFFER_COUNT_PASS", 1);

    for (auto& d : this->drawables)
        if (d->isVisible())
            d->draw(this->stateSet);

    modes.clear("DYNAMIC_FRAGMENT_BUFFER_COUNT_PASS");


    /*
     * Compute prefix sum using compute shader.
     */
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->numFragmentsBuffer);

    uniforms.setImageTexture(0, this->countingBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
    uniforms.setImageTexture(1, this->offsetBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    this->stateSet.apply(this->prefixSumProgram);
    glDispatchCompute(1, 1, 1);

    // Read number of fragments
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->numFragmentsBuffer);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    uint numFragments;
    memcpy(&numFragments, p, sizeof(uint));
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


    // CPU-based prefix sum (slower than compute shader)
    /*
    GLuint* countData = new GLuint[width * height];
    GLuint* offsetData = new GLuint[width * height];

    glBindTexture(GL_TEXTURE_2D, this->countingBuffer);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, countData);

    // Compute prefix sum
    offsetData[0] = 0;
    for (unsigned  i = 1; i < width * height; ++i)
        offsetData[i] = offsetData[i - 1] + countData[i - 1];

    GLuint numFragments = offsetData[width * height - 1] + countData[width + height - 1];

    for (unsigned  i = 0; i < width * height; ++i)
        countData[i] = 0;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, countData);

    glBindTexture(GL_TEXTURE_2D, this->offsetBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, offsetData);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] countData;
    delete[] offsetData;
    */


    /*
     * Render all drawables into dynamic fragment buffer
     */
    // Resize buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->fragmentBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, numFragments * 8 * sizeof(GLfloat), 0, GL_DYNAMIC_DRAW);  // 8 floats: RGBA + depth + padding

    uniforms.setImageTexture(0, this->countingBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
    uniforms.setImageTexture(1, this->offsetBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);

    modes.set("DYNAMIC_FRAGMENT_BUFFER_RENDER_PASS", 1);

    for (auto& d : this->drawables)
        if (d->isVisible())
            d->draw(this->stateSet);

    modes.clear("DYNAMIC_FRAGMENT_BUFFER_RENDER_PASS");


    /*
     * Sort and display fragments in dynamic fragment buffer.
     */
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->fragmentBuffer);

    uniforms.setImageTexture(0, this->countingBuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);
    uniforms.setImageTexture(1, this->offsetBuffer, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
    uniforms.set("backgroundColor", this->backgroundColor);
    uniforms.set("checkerboard", this->checkerboard);

    this->stateSet.apply(this->displayProgram);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebuffer.getFramebuffer());

    glBindVertexArray(this->fullScreenQuadVAO);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    return this->framebuffer;
}


/**
 * @brief Resize buffers.
 */
bool TransparentRenderStageDFB::resize(unsigned int width, unsigned int height)
{
    // Check if we even need to resize
    if( width == this->width && height == this->height )
        return false;

    // Resize counting buffer
    glBindTexture(GL_TEXTURE_2D, this->countingBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Resize offset buffer
    glBindTexture(GL_TEXTURE_2D, this->offsetBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Resize framebuffer
    this->framebuffer.resize(width, height, 0, GL_TEXTURE_RECTANGLE, GL_RGBA8, GL_TEXTURE_RECTANGLE, GL_DEPTH_COMPONENT32);

    // Remember values
    this->width  = width;
    this->height = height;

    return true;
}


/**
 * @brief Adds a drawable.
 * @param drawable
 */
void TransparentRenderStageDFB::addDrawable(Drawable* drawable)
{
    this->drawables.push_back(drawable);
}

/**
 * @brief Removes given drawable.
 * @param drawable
 */
void TransparentRenderStageDFB::removeDrawable(Drawable* drawable)
{
    this->drawables.erase(std::remove(this->drawables.begin(), this->drawables.end(), drawable), this->drawables.end());
}

/**
 * @brief Returns the list of drawables in current render stage.
 * @return
 */
const std::vector<Drawable*>& TransparentRenderStageDFB::getDrawables() const
{
    return this->drawables;
}


/**
 * @brief Adds a volume for volume rendering.
 * @param volume
 */
void TransparentRenderStageDFB::addVolume(Volume* /*volume*/)
{
    std::cout << "Warning: Volumes are not supported yet in the dynamic fragment buffer" << std::endl;
}

/**
 * @brief Sets the drawing of the background checkerboard.
 * @param showCheckerboard
 */
void TransparentRenderStageDFB::setCheckerboard(bool showCheckerboard)
{
    this->checkerboard = showCheckerboard;
}


/**
 * @brief Checks if the background checkerboard is enabled.
 * @return
 */
bool TransparentRenderStageDFB::getCheckerboard()
{
    return this->checkerboard;
}


/**
 * @brief Sets the background color if the checkerboard is disabled.
 * @param color
 */
void TransparentRenderStageDFB::setBackgroundColor(const glm::vec4& color)
{
    this->backgroundColor = color;
}


/**
 * @brief Returns the background color which is used if the checkerboard is disabled.
 * @return
 */
glm::vec4 TransparentRenderStageDFB::getBackgroundColor()
{
    return this->backgroundColor;
}

}
