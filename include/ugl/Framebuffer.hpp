/** @file Framebuffer.hpp



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

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <GL/glew.h>
#include <vector>

namespace ugl
{

/**
 * @brief FBO creation, completeness check and management.
 */
class Framebuffer
{
public:

    /*
     * Struct describing color textures
     */
    struct ColorTextureConf {
        ColorTextureConf() : format(GL_RGBA), type(GL_UNSIGNED_BYTE) {}
        GLenum target;
        GLint internalFormat;
        GLenum format;
        GLenum type;
    };

    /*
     * Struct describing depth texture
     */
    struct DepthTextureConf {
        DepthTextureConf() : format(GL_DEPTH_COMPONENT), type(GL_UNSIGNED_BYTE)  {}
        GLenum target;
        GLint internalFormat;
        GLenum format;
        GLenum type;
    };

    Framebuffer();
    virtual ~Framebuffer();

    void resize(unsigned int width, unsigned int height, unsigned int numSamples, GLenum colorTarget, GLint colorFormat, GLenum depthTarget, GLint depthIntFormat);
    void resize(unsigned int width, unsigned int height, unsigned int numSamples, GLenum depthTarget, GLint depthIntFormat);
    bool checkCompleteness();

    void resolve();

    void destroy();

    unsigned int addColorTexture(GLenum target, GLint internalFormat, GLenum format, GLenum type);
    bool setColorTextureTarget(unsigned int index, GLenum target);
    bool setColorTextureInternalFormat(unsigned int index, GLint internalFormat);
    bool setColorTextureFormat(unsigned int index, GLenum format);
    bool setColorTextureType(unsigned int index, GLenum type);

    unsigned int getWidth();
    unsigned int getHeight();
    unsigned int getNumSamples();

    GLuint getFramebuffer();
    GLuint getColorTexture();
    GLuint getColorTexture(unsigned int index);
    const std::vector<GLuint>* getColorTextures();
    GLenum getColorTextureTarget(unsigned int index);
    GLint getColorTextureInternalFormat(unsigned int index);
    GLenum getColorTextureFormat(unsigned int index);
    GLenum getColorTextureType(unsigned int index);
    GLuint getDepthTexture();

private:
    unsigned int width;
    unsigned int height;
    unsigned int samples;

    GLuint fbo;
    std::vector<GLuint> colorTextures;
    GLuint depthTexture;

    std::vector<ColorTextureConf*> colorTextureConfs;
    DepthTextureConf *depthTextureConf;
};

}

#endif // FRAMEBUFFER_HPP
