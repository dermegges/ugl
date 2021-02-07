/** @file Framebuffer.cpp



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

#include "ugl/Framebuffer.hpp"
#include "ugl/ErrorCheck.hpp"

#include <iostream>


namespace ugl
{

/*
 * Constructor.
 */
Framebuffer::Framebuffer()
{
    this->fbo = 0;
    this->depthTexture = 0;
    this->depthTextureConf = new DepthTextureConf;

    this->samples = 0;
    this->width = 0;
    this->height = 0;
}


/*
 * Destructor.
 */
Framebuffer::~Framebuffer()
{    
}


/**
 * @brief build and Resize textures.
 * @param width
 * @param height
 * @param numSamples
 * @param colorFormat
 * @param depthFormat
 * color Target and Format are applied to the last color texture added. If none was added, one is created 
 */
void Framebuffer::resize(unsigned int width, unsigned int height, unsigned int numSamples, GLenum colorTarget, GLint colorFormat, GLenum depthTarget, GLint depthIntFormat)
{
    /*
     * Color texture
     */
    if(this->colorTextureConfs.empty()) //no color Textures yet
    {
        this->colorTextureConfs.push_back(new ColorTextureConf);
    }

    unsigned int lastTex = colorTextureConfs.size()-1;

    this->colorTextureConfs[lastTex]->target = colorTarget;
    this->colorTextureConfs[lastTex]->internalFormat = colorFormat;

    this->colorTextures.resize(colorTextureConfs.size(), 0);

    for(unsigned int i=0; i<colorTextures.size(); i++)
    {
        ColorTextureConf curCConf = *(this->colorTextureConfs[i]);

        if(this->colorTextures[i] == 0)
            glGenTextures(1, &this->colorTextures[i]);

        glBindTexture(curCConf.target, this->colorTextures[i]);

        if (numSamples > 0)
        {
            glTexImage2DMultisample(curCConf.target, numSamples, curCConf.internalFormat, width, height, GL_TRUE);
        }
        else
        {
            glTexImage2D(curCConf.target, 0, curCConf.internalFormat, width, height, 0, curCConf.format, curCConf.type, 0);
        }
        
        checkGLError();

        glBindTexture(curCConf.target, 0);
    }


    /*
     * Depth texture
     */
    this->depthTextureConf->target          = depthTarget;
    this->depthTextureConf->internalFormat  = depthIntFormat;
    //this->depthTextureConf->format          = depthFormat;
    //this->depthTextureConf->type            = depthType;

    if (this->depthTexture == 0)
        glGenTextures(1, &this->depthTexture);

    glBindTexture(this->depthTextureConf->target, this->depthTexture);

    if (numSamples > 0)
    {
        glTexImage2DMultisample(this->depthTextureConf->target, numSamples, this->depthTextureConf->internalFormat, width, height, GL_TRUE);
    }
    else
    {
        glTexImage2D(this->depthTextureConf->target, 0, this->depthTextureConf->internalFormat, width, height, 0, this->depthTextureConf->format, this->depthTextureConf->type, 0);
    }

    glBindTexture(this->depthTextureConf->target, 0);


    /*
     * Framebuffer object
     */
    if (this->fbo == 0)
    {
        glGenFramebuffers(1, &this->fbo);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    for(unsigned int i=0; i<this->colorTextures.size(); i++)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, this->colorTextures[i], 0);
    }
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    /*
     * Store parameters
     */
    this->width = width;
    this->height = height;
    this->samples = numSamples;
}

/**
 * @brief build and Resize textures.
 * @param width
 * @param height
 * @param numSamples
 * @param depthIntFormat
 * should be used if color textures have been added already
 */
void Framebuffer::resize(unsigned int width, unsigned int height, unsigned int numSamples, GLenum depthTarget, GLint depthIntFormat)
{  
    //check if a color texture exists
    if (this->colorTextureConfs.empty())
    {
        resize(width, height, numSamples, GL_TEXTURE_RECTANGLE, GL_RGBA8, depthTarget, depthIntFormat);
        return;
    }

    /*
     * Color texture
     */
    this->colorTextures.resize(colorTextureConfs.size(), 0);

    for(unsigned int i=0; i<colorTextures.size(); i++)
    {
        ColorTextureConf curCConf = *(this->colorTextureConfs[i]);

        if(this->colorTextures[i] == 0)
            glGenTextures(1, &this->colorTextures[i]);

        //glActiveTexture(GL_TEXTURE0+i);

        glBindTexture(curCConf.target, this->colorTextures[i]);

        if (numSamples > 0)
        {
            glTexImage2DMultisample(curCConf.target, numSamples, curCConf.internalFormat, width, height, GL_TRUE);
        }
        else
        {
            glTexImage2D(curCConf.target, 0, curCConf.internalFormat, width, height, 0, curCConf.format, curCConf.type, 0);
        }
        glTexParameteri(curCConf.target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(curCConf.target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(curCConf.target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(curCConf.target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        checkGLError();
        glBindTexture(curCConf.target, 0);
    }


    /*
     * Depth texture
     */
    this->depthTextureConf->target          = depthTarget;
    this->depthTextureConf->internalFormat  = depthIntFormat;
    //this->depthTextureConf->format          = depthFormat;
    //this->depthTextureConf->type            = depthType;

    if (this->depthTexture == 0)
        glGenTextures(1, &this->depthTexture);

    //glActiveTexture(GL_TEXTURE0+colorTextures.size());
    glBindTexture(this->depthTextureConf->target, this->depthTexture);

    if (numSamples > 0)
    {
        glTexImage2DMultisample(this->depthTextureConf->target, numSamples, this->depthTextureConf->internalFormat, width, height, GL_TRUE);
    }
    else
    {
        glTexImage2D(this->depthTextureConf->target, 0, this->depthTextureConf->internalFormat, width, height, 0, this->depthTextureConf->format, this->depthTextureConf->type, 0);
    }

    glTexParameteri(this->depthTextureConf->target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(this->depthTextureConf->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(this->depthTextureConf->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(this->depthTextureConf->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(this->depthTextureConf->target, 0);


    /*
     * Framebuffer object
     */
    if (this->fbo == 0)
    {
        glGenFramebuffers(1, &this->fbo);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    std::vector<GLuint> drawBuffers;
    for(unsigned int i=0; i<this->colorTextures.size(); i++)
    {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, this->colorTextures[i], 0);
        drawBuffers.push_back(GL_COLOR_ATTACHMENT0+i);
        checkGLError();
    }
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*
     * Store parameters
     */
    this->width = width;
    this->height = height;
    this->samples = numSamples;
}


/**
 * @brief Check framebuffer completeness after resize.
 * @return
 */
bool Framebuffer::checkCompleteness()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        return true;
    }
    else if (status == GL_FRAMEBUFFER_UNSUPPORTED)
    {
        std::cerr << "Error: Framebuffer format not supported (GL_FRAMEBUFFER_UNSUPPORTED)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    {
        std::cerr << "Error: Framebuffer missing attachment (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    {
        std::cerr << "Error: Framebuffer incomplete attachment (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
    {
        std::cerr << "Error: Framebuffer attached images must have same dimensions (GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)
    {
        std::cerr << "Error: Framebuffer attached images must have same format (GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
    {
        std::cerr << "Error: Framebuffer missing draw buffer (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)" << std::endl;
        return false;
    }
    else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
    {
        std::cerr << "Error: Framebuffer missing read buffer (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)" << std::endl;
        return false;
    }
    else
    {
        std::cerr << "Error: Framebuffer incomplete (Unknown error: " << status << ")" << std::endl;
        return false;
    }
}


/**
 * @brief Blits the framebuffer to the default framebuffer (resolve for MSAA).
 */
void Framebuffer::resolve()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
    glBlitFramebuffer(0, 0, this->width, this->height, 0, 0, this->width, this->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}


/**
 * @brief Deletes all resources.
 */
void Framebuffer::destroy()
{
    if (this->fbo != 0)
    {
        glDeleteFramebuffers(1, &this->fbo);
        this->fbo = 0;
    }

    for(unsigned int i=0; i<colorTextures.size(); i++)
    {
        if (this->colorTextures[i] != 0)
        {
            glDeleteTextures(1, &this->colorTextures[i]);
            this->colorTextures[i] = 0;
        }
    }

    if (this->depthTexture != 0)
    {
        glDeleteTextures(1, &this->depthTexture);
        this->depthTexture = 0;
    }
}

/**
 * @brief Sets new color texture configuration, 
 * returns number of the new texture
 */
unsigned int Framebuffer::addColorTexture(GLenum target, GLint internalFormat, GLenum format, GLenum type)
{
    ColorTextureConf *newConf = new ColorTextureConf();
    newConf->target         = target;
    newConf->internalFormat = internalFormat;
    newConf->format         = format;
    newConf->type           = type;

    this->colorTextureConfs.push_back(newConf);

    return this->colorTextureConfs.size()-1;
}

/**
 * @brief Sets target of existing colorTexture, 
 * returns if it was successful
 */
bool Framebuffer::setColorTextureTarget(unsigned int index, GLenum target)
{
    if(index >= this->colorTextureConfs.size()) //colortexture does not exist
        return(false);

    this->colorTextureConfs[index]->target = target;
    return true;
}

/**
 * @brief Sets internalFormat of existing colorTexture, 
 * returns if it was successful
 */
bool Framebuffer::setColorTextureInternalFormat(unsigned int index, GLint internalFormat)
{
    if(index >= this->colorTextureConfs.size()) //colortexture does not exist
        return(false);

    this->colorTextureConfs[index]->internalFormat = internalFormat;
    return true;
}

/**
 * @brief Sets format of existing colorTexture, 
 * returns if it was successful
 */
bool Framebuffer::setColorTextureFormat(unsigned int index, GLenum format)
{
    if(index >= this->colorTextureConfs.size()) //colortexture does not exist
        return(false);

    this->colorTextureConfs[index]->format = format;
    return true;
}

/**
 * @brief Sets type of existing colorTexture, 
 * returns if it was successful
 */
bool Framebuffer::setColorTextureType(unsigned int index, GLenum type)
{
    if(index >= this->colorTextureConfs.size()) //colortexture does not exist
        return(false);

    this->colorTextureConfs[index]->type = type;
    return true;
}

/**
 * @brief Returns the width of the framebuffer.
 * @return
 */
unsigned int Framebuffer::getWidth()
{
    return this->width;
}


/**
 * @brief Returns the height of the framebuffer.
 * @return
 */
unsigned int Framebuffer::getHeight()
{
    return this->height;
}


/**
 * @brief Returns the number of samples of the framebuffer.
 * @return
 */
unsigned int Framebuffer::getNumSamples()
{
    return this->samples;
}


/**
 * @brief Returns the framebuffer object id.
 * @return
 */
GLuint Framebuffer::getFramebuffer()
{
    return this->fbo;
}


/**
 * @brief Returns the last color texture id.
 * @return
 */
GLuint Framebuffer::getColorTexture()
{
    if(this->colorTextures.empty())
        return 0;

    return this->colorTextures.back();
}

/**
 * @brief Returns the chosen color texture id.
 * @return
 */
GLuint Framebuffer::getColorTexture(unsigned int index)
{
    if(index >= this->colorTextures.size())
        return 0;

    return this->colorTextures[index];
}

/**
 * @brief Returns array of color texture ids.
 * @return
 */
const std::vector<GLuint>* Framebuffer::getColorTextures()
{
    return &(this->colorTextures);
}

GLenum Framebuffer::getColorTextureTarget(unsigned int index)
{
    if(index >= this->colorTextures.size())
        return 0;

    return this->colorTextureConfs[index]->target;
}

GLint Framebuffer::getColorTextureInternalFormat(unsigned int index)
{
    if(index >= this->colorTextures.size())
        return 0;

    return this->colorTextureConfs[index]->internalFormat;
}

GLenum Framebuffer::getColorTextureFormat(unsigned int index)
{
    if(index >= this->colorTextures.size())
        return 0;

    return this->colorTextureConfs[index]->format;
}

GLenum Framebuffer::getColorTextureType(unsigned int index)
{
    if(index >= this->colorTextures.size())
        return 0;

    return this->colorTextureConfs[index]->type;
}


/**
 * @brief Returns the depth texture id.
 * @return
 */
GLuint Framebuffer::getDepthTexture()
{
    return this->depthTexture;
}

}
