/** @file ShaderProgram.hpp



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

#ifndef _SHADERPROGRAM_HPP_
#define _SHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <boost/utility.hpp>

#include <string>
#include <vector>

namespace ugl
{

/**
 * Compiles, links, binds and manages a GLSL program (similar to Qt's
 * QOpenGLShaderProgram).
 *
 * Errors are directly printed to stderr.
 */
class ShaderProgram : private boost::noncopyable
{
public:
    ShaderProgram();
    ~ShaderProgram();

    GLuint programId() const;

    bool addShaderFromSourceCode(GLenum shaderType, const std::string& source);
    void bindAttributeLocation(const std::string& name, GLuint index);
    bool link();
    void bind();
    void release();

private:
    GLuint  m_program;

    template <typename GetObjectFunction, typename GetObjectInfoLogFunction>
    static bool checkStatus(
            GLuint object, GLenum pname, const std::string& errorText,
            GetObjectFunction getObject,
            GetObjectInfoLogFunction getObjectInfoLog);

    static std::string getNameOfShaderType(GLenum shaderType);
};

// -------------------------------------------------------------------------

inline ShaderProgram::ShaderProgram() :
    m_program(glCreateProgram())
{
}

// -------------------------------------------------------------------------

inline ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_program);
}

// -------------------------------------------------------------------------

inline GLuint ShaderProgram::programId() const
{
    return m_program;
}

// -------------------------------------------------------------------------

inline void ShaderProgram::bindAttributeLocation(
        const std::string& name, GLuint index)
{
    glBindAttribLocation(m_program, index, name.c_str());
}

// -------------------------------------------------------------------------

inline void ShaderProgram::bind()
{
    glUseProgram(m_program);
}

// -------------------------------------------------------------------------

inline void ShaderProgram::release()
{
    glUseProgram(0u);
}

} // namespace ugl
#endif // _SHADERPROGRAM_HPP_
