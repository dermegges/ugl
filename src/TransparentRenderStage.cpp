/** @file TransparentRenderStage.cpp



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

#include "ugl/TransparentRenderStage.hpp"
#include "ugl/ErrorCheck.hpp"

#include <iostream>
#include <sstream>

namespace ugl
{


/**
 * @brief Constructor.
 * @param numSamples
 */
TransparentRenderStage::TransparentRenderStage(unsigned int numSamples, unsigned int maxPasses, unsigned int samplesThreshold) : volume(0), samplesThreshold(samplesThreshold)
{
    this->setNumSamples(numSamples);
    this->setMaxPasses(maxPasses);

    this->setBackgroundColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    this->setCheckerboard(true);

    this->stateSet.getOrCreateModes().set("DEPTH_PEELING", 1);

    // Initialize full screen quad and blend program
    glGenVertexArrays(1, &this->fullScreenQuadVAO);

    // Blend program
    this->blendProgram.addImportPath("shader");
    this->blendProgram.addShaderFromSourceFile( COMBINED, "ugl/blend.glsl" );

    // set default width to resize immediately on first render
    this->width  = 0;
    this->height = 0;
}


/**
 * @brief Resizes the required framebuffers and textures.
 * @param width
 * @param height
 */
void TransparentRenderStage::resize( unsigned int width, unsigned int height )
{
    // Check if we even need to resize
    if( width == this->width && height == this->height )
        return;

    // Resize buffers
    for (unsigned int i = 0; i < 2; ++i)
    {
        this->peelBuffers[i].resize(width, height, this->numSamples, this->textureTarget, GL_RGBA8, this->textureTarget, GL_DEPTH_COMPONENT32);
        this->peelBuffers[i].checkCompleteness();

        this->targetBuffers[i].resize(width, height, this->numSamples, this->textureTarget, GL_RGBA8, this->textureTarget, GL_DEPTH_COMPONENT32);
        this->targetBuffers[i].checkCompleteness();
    }

    // Restore state
    glBindTexture(this->textureTarget, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Remember values
    this->width  = width;
    this->height = height;
}


/**
 * @brief Draws everything using depth peeling and automatically resolves the target buffer.
 * @param state
 */
void TransparentRenderStage::draw(const StateSet& state)
{
    this->drawOffscreen(state).resolve();
}


/**
 * @brief Draws everything using depth peeling and returns the target buffer.
 * @param state
 */
Framebuffer& TransparentRenderStage::drawOffscreen(const StateSet& state)
{   
    this->stateSet.setParent(state);

    const glm::uvec4& viewport = *(state.getViewport());
    const unsigned int width  = viewport[2];
    const unsigned int height = viewport[3];

    this->resize(width, height);

    // Clear buffers
    for (unsigned int i = 0; i < 2; ++i)
    {
        // Target buffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->targetBuffers[i].getFramebuffer());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Peel buffer
        glBindFramebuffer(GL_FRAMEBUFFER, this->peelBuffers[i].getFramebuffer());
        glClearDepth(0.0);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    GLuint query;
    glGenQueries(1, &query);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    if (this->numSamples > 0)
    {
        glEnable(GL_SAMPLE_SHADING);
        glMinSampleShading(1.0f);
    }
    else
    {
        glDisable(GL_SAMPLE_SHADING);
    }

    // Perform peeling
    Framebuffer* resultBuffer = 0;
    for(unsigned int peelPass = 0; peelPass < this->maxPasses; ++peelPass)
    {
        bool firstPass = (peelPass == 0);
        bool lastPass;

        /*
         * Render to peel target
         */
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        // Render target
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->peelBuffers[peelPass % 2].getFramebuffer());

        // Discard source
        this->stateSet.getOrCreateUniforms().setTexture("depthPeeling.discardDepth", this->textureTarget, this->peelBuffers[(peelPass + 1) % 2].getDepthTexture());

        // Start drawing
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBeginQuery(GL_SAMPLES_PASSED, query);

        // Drawables
        for(std::vector<Drawable*>::iterator drawable = this->drawables.begin(); drawable != this->drawables.end(); ++drawable)
            if ((*drawable)->isVisible())
                (*drawable)->draw(this->stateSet);

        // Volume rendering bounding box (depth only)
        if (this->volume != 0 && this->volume->isVisible() && this->volume->getRenderBoundingBoxDepthCube())
        {
            glColorMask(false, false, false, false);
            this->volume->getBoundingBoxDrawable().draw(this->stateSet);
            glColorMask(true, true, true, true);
        }

        glEndQuery(GL_SAMPLES_PASSED);

        GLuint samples;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &samples);

        lastPass = (peelPass == (this->maxPasses - 1)) || (samples < this->samplesThreshold);


        /*
         * Blend the peel color buffer into the target framebuffer
         */
        resultBuffer = &this->targetBuffers[peelPass % 2];
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resultBuffer->getFramebuffer());
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);

        ModeSet& modes = this->stateSet.getOrCreateModes();
        UniformSet& uniforms = this->stateSet.getOrCreateUniforms();
        uniforms.setTexture("previousTarget", this->textureTarget, this->targetBuffers[(peelPass + 1) % 2].getColorTexture());
        uniforms.setTexture("colorTex", this->textureTarget, this->peelBuffers[peelPass % 2].getColorTexture());
        uniforms.set("firstPass", firstPass);
        uniforms.set("lastPass", lastPass);
        uniforms.set("backgroundColor", this->backgroundColor);
        uniforms.set("checkerboard", this->checkerboard);
        uniforms.set("viewport", glm::vec4(viewport));

        modes.clear("VOLUMERENDERING");
        if (this->volume != 0 && this->volume->isVisible())
        {
            GLuint intensityTexture = this->volume->getIntensityTexture();
            GLuint transferTextureArray = this->volume->getTransferArrayTexture();

            // Only enabled volume rendering if we have valid textures
            if (intensityTexture > 0 && transferTextureArray > 0)
            {
                ugl::BoundingBox boundingBox = this->volume->getBoundingBox();
                uniforms.set("volumeRendering.boxMin", boundingBox.getMin());
                uniforms.set("volumeRendering.boxMax", boundingBox.getMax());
                uniforms.set("volumeRendering.sampleStepSize", this->volume->getSampleStepSize());
                uniforms.set("volumeRendering.hasMask", this->volume->hasMaskTexture());
                uniforms.setTexture("volumeRendering.intensity", GL_TEXTURE_3D, intensityTexture);
                uniforms.set("volumeRendering.intensityResolution", this->volume->getIntensityResolution());
                uniforms.setTexture("volumeRendering.mask", GL_TEXTURE_3D, this->volume->getMaskTexture());
                uniforms.set("volumeRendering.maskResolution", this->volume->getMaskResolution());

                uniforms.set("volumeRendering.transferResolution", this->volume->getTransferResolution());
                uniforms.setTexture("volumeRendering.transfer", GL_TEXTURE_1D_ARRAY, transferTextureArray);
                uniforms.set("volumeRendering.alpha", this->volume->getAlpha());
                uniforms.set("volumeRendering.numTransfer", std::max(0, std::min(this->volume->getNumTransferTextures(), 4)));

                uniforms.setTexture("volumeRendering.currentDepthTex", this->textureTarget, this->peelBuffers[peelPass % 2].getDepthTexture());
                uniforms.setTexture("volumeRendering.previousDepthTex", this->textureTarget, this->peelBuffers[(peelPass + 1) % 2].getDepthTexture());

                glm::vec2 depthRange;
                glGetFloatv(GL_DEPTH_RANGE, &depthRange.x);
                uniforms.set("volumeRendering.depthRange", depthRange);

                modes.set("VOLUMERENDERING", 1);
            }
        }

        checkGLError();

        this->stateSet.apply(this->blendProgram);

        checkGLError();

        glBindVertexArray(this->fullScreenQuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        checkGLError();

        if(lastPass)
            break;
    }

    if (this->numSamples > 0)
        glDisable(GL_SAMPLE_SHADING);

    glDeleteQueries(1, &query);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Return target buffer
    return *resultBuffer;
}


/**
 * @brief Adds a drawable which will be rendered using depth peeling.
 * @param drawable
 */
void TransparentRenderStage::addDrawable(Drawable* drawable)
{
    this->drawables.push_back(drawable);
}

/**
 * @brief Removes given drawable.
 * @param drawable
 */
void TransparentRenderStage::removeDrawable(Drawable* drawable)
{
    this->drawables.erase(std::remove(this->drawables.begin(), this->drawables.end(), drawable), this->drawables.end());
}

/**
 * @brief Returns the number of samples.
 * @return
 */
unsigned int TransparentRenderStage::getNumSamples()
{
    return this->numSamples;
}


/**
 * @brief Sets the number of samples.
 * @param numSamples
 */
void TransparentRenderStage::setNumSamples(unsigned int numSamples)
{            
    // Check if number of samples is supported.
    int maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

    if (numSamples > (unsigned int) maxSamples)
    {
        std::cerr << "Warning: Number of samples clamped to " << maxSamples << " (maximum)!" << std::endl;
        this->numSamples = maxSamples;
    }
    else
    {
        this->numSamples = numSamples;
    }

    // Force creation of new framebuffers
    this->peelBuffers[0].destroy();
    this->peelBuffers[1].destroy();
    this->targetBuffers[0].destroy();
    this->targetBuffers[1].destroy();

    // Update state set
    if (this->numSamples > 0)
        this->stateSet.getOrCreateModes().set("MSAA", 1);
    else
        this->stateSet.getOrCreateModes().clear("MSAA");

    // Update texture target
    this->textureTarget = (this->numSamples > 0) ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_RECTANGLE;

    // Force resize
    this->width = 0;
    this->height = 0;
}


/**
 * @brief Returns the current depth peeling passes threshold.
 * @return
 */
unsigned int TransparentRenderStage::getMaxPasses()
{
    return this->maxPasses;
}


/**
 * @brief Sets the depth peeling passes threshold.
 * @param maxPasses
 */
void TransparentRenderStage::setMaxPasses(unsigned int maxPasses)
{
    this->maxPasses = maxPasses;
}


/**
 * @brief Sets the drawing of the background checkerboard.
 * @param showCheckerboard
 */
void TransparentRenderStage::setCheckerboard(bool showCheckerboard)
{
    this->checkerboard = showCheckerboard;
}


/**
 * @brief Checks if the background checkerboard is enabled.
 * @return
 */
bool TransparentRenderStage::getCheckerboard()
{
    return this->checkerboard;
}


/**
 * @brief Sets the background color if the checkerboard is disabled.
 * @param color
 */
void TransparentRenderStage::setBackgroundColor(const glm::vec4& color)
{
    this->backgroundColor = color;
}


/**
 * @brief Returns the background color which is used if the checkerboard is disabled.
 * @return
 */
glm::vec4 TransparentRenderStage::getBackgroundColor()
{
    return this->backgroundColor;
}


/**
 * @brief Adds a volume for volume rendering.
 * @param volume
 */
void TransparentRenderStage::addVolume(Volume* volume)
{
    if (this->volume != 0)
    {
        std::cout << "Warning: Currently only one volume supported for volume rendering!" << std::endl;
        return;
    }

    this->volume = volume;
}

/**
 * @brief returns list of drawables in current render stage
 * @return
 */
const std::vector<Drawable*>* TransparentRenderStage::getDrawables()
{
    return &(this->drawables);
}


}
