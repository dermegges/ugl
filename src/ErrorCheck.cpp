/** @file ErrorCheck.cpp



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

#include "ugl/ErrorCheck.hpp"

#include <GL/glew.h>

#include <iostream>
#include <assert.h>

namespace ugl
{

void checkAndPrintGLError(std::string label, const char* file, int line )
{
    std::string errors;

    GLenum error;    
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        const char* which = "UNKNOWN";

        switch( error )
        {
            case GL_INVALID_ENUM: which = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: which = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: which = "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: which = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: which = "GL_OUT_OF_MEMORY"; break;
            case GL_STACK_UNDERFLOW: which = "GL_STACK_UNDERFLOW"; break;
            case GL_STACK_OVERFLOW: which = "GL_STACK_OVERFLOW"; break;
        }

        if (errors.size() > 0)
            errors += ", ";

        errors += std::string(which);
    }

    if (errors.size() > 0)
    {
        if (label.size() > 0)
            std::cerr << "OpenGL Error: " << errors << " (Label: \"" << label << "\", File: " << file << ":" << line << ")" << std::endl;
        else
            std::cerr << "OpenGL Error: " << errors << " (File: " << file << ":" << line << ")" << std::endl;

#ifdef UGL_ASSERT_ON_OPENGL_ERROR
        assert(false);
#endif
    }
}

}
