/** @file ShaderProgram.cpp



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

#include "ugl/ShaderProgram.hpp"

#include <iostream>

namespace ugl
{

bool ShaderProgram::addShaderFromSourceCode(
        GLenum shaderType, const std::string& source)
{
    GLuint shader = glCreateShader(shaderType);

    const char *sourceData = source.c_str();
    glShaderSource(shader, 1, &sourceData, nullptr);

    glCompileShader(shader);

    bool success = checkStatus(shader, GL_COMPILE_STATUS,
            "Could not compile " + getNameOfShaderType(shaderType) + " shader",
            glGetShaderiv, glGetShaderInfoLog);

    if (success)
        glAttachShader(m_program, shader);

    glDeleteShader(shader);
    return success;
}

// -------------------------------------------------------------------------

bool ShaderProgram::link()
{
    glLinkProgram(m_program);

    return checkStatus(m_program, GL_LINK_STATUS,
           "Could not link shader program",
           glGetProgramiv, glGetProgramInfoLog);
}

// -------------------------------------------------------------------------

template <typename GetObjectFunction, typename GetObjectInfoLogFunction>
bool ShaderProgram::checkStatus(
        GLuint object, GLenum pname, const std::string& errorText,
        GetObjectFunction getObject,
        GetObjectInfoLogFunction getObjectInfoLog)
{
    GLint status;
    getObject(object, pname, &status);

    bool out;
    if (status == GL_TRUE)
    {
        out = true;
    }
    else
    {
        GLint infoLogLength;
        getObject(object, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *infoLog = new GLchar[infoLogLength];
        getObjectInfoLog(object, infoLogLength, nullptr, infoLog);

        std::cerr << errorText << ":\n" << infoLog << '\n';

        delete[] infoLog;
        out = false;
    }

    return out;
}

// -------------------------------------------------------------------------

std::string ShaderProgram::getNameOfShaderType(GLenum shaderType)
{
    switch (shaderType)
    {
        case GL_TESS_EVALUATION_SHADER: return "Tess_evaluation";
        case GL_TESS_CONTROL_SHADER:    return "Tess_control";
        case GL_VERTEX_SHADER:          return "Vertex";
        case GL_FRAGMENT_SHADER:        return "Fragment";
        case GL_GEOMETRY_SHADER:        return "Geometry";
        default: return "unknown";
    }
}

} // namespace ugl
