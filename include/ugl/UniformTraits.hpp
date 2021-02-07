/** @file UniformTraits.hpp



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

#ifndef _UNIFORMTRAITS_HPP_
#define _UNIFORMTRAITS_HPP_

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace ugl
{

// -------------------------------------------------------------------------

/**
 * Traits for getting the size and a data pointer of different possible input
 * data to UniformSet.
 */
template<typename T>
struct uniform_traits
{
};

// --- basic types ---------------------------------------------------------

template<typename T>
struct basic_uniform_traits
{
    static const GLvoid* data( const T& t ) { return &t; }
    static GLsizei size( const T& /*t*/ ) { return 1; }
};

template<>
struct uniform_traits<int> : public basic_uniform_traits<int>
{
    static GLenum type() { return GL_INT; }
};

template<>
struct uniform_traits<unsigned int> : public basic_uniform_traits<unsigned int>
{
    static GLenum type() { return GL_UNSIGNED_INT; }
};

template<>
struct uniform_traits<float> : public basic_uniform_traits<float>
{
    static GLenum type() { return GL_FLOAT; }
};

template<>
struct uniform_traits<double> : public basic_uniform_traits<double>
{
    static GLenum type() { return GL_DOUBLE; }
};

template<>
struct uniform_traits<bool> : public basic_uniform_traits<bool>
{
    static GLenum type() { return GL_BOOL; }
};

// --- glm types -----------------------------------------------------------

template<typename T>
struct glm_uniform_traits_base
{
    static const GLvoid* data( const T& m ) { return glm::value_ptr( m ); }
    static GLsizei size( const T& /*t*/ ) { return 1; }
};

template<>
struct uniform_traits<glm::vec2> : glm_uniform_traits_base<glm::vec2>
{
    static GLenum type() { return GL_FLOAT_VEC2; }
};

template<>
struct uniform_traits<glm::vec3> : glm_uniform_traits_base<glm::vec3>
{
    static GLenum type() { return GL_FLOAT_VEC3; }
};

template<>
struct uniform_traits<glm::vec4> : glm_uniform_traits_base<glm::vec4>
{
    static GLenum type() { return GL_FLOAT_VEC4; }
};

template<>
struct uniform_traits<glm::ivec2> : glm_uniform_traits_base<glm::ivec2>
{
    static GLenum type() { return GL_INT_VEC2; }
};

template<>
struct uniform_traits<glm::ivec3> : glm_uniform_traits_base<glm::ivec3>
{
    static GLenum type() { return GL_INT_VEC3; }
};

template<>
struct uniform_traits<glm::ivec4> : glm_uniform_traits_base<glm::ivec4>
{
    static GLenum type() { return GL_INT_VEC4; }
};

template<>
struct uniform_traits<glm::uvec2> : glm_uniform_traits_base<glm::uvec2>
{
    static GLenum type() { return GL_INT_VEC2; }
};

template<>
struct uniform_traits<glm::uvec3> : glm_uniform_traits_base<glm::uvec3>
{
    static GLenum type() { return GL_INT_VEC3; }
};

template<>
struct uniform_traits<glm::uvec4> : glm_uniform_traits_base<glm::uvec4>
{
    static GLenum type() { return GL_INT_VEC4; }
};

template<>
struct uniform_traits<glm::mat3> : glm_uniform_traits_base<glm::mat3>
{
    static GLenum type() { return GL_FLOAT_MAT3; }
};

template<>
struct uniform_traits<glm::mat4> : glm_uniform_traits_base<glm::mat4>
{
    static GLenum type() { return GL_FLOAT_MAT4; }
};

// --- STL vector ----------------------------------------------------------

template<typename U>
struct uniform_traits< std::vector<U> >
{
    static const GLvoid* data( const std::vector<U>& vector )
    { return vector.data(); }

    static GLsizei size( const std::vector<U>& vector )
    {
        GLsizei out = 0;
        for( const U& element : vector )
            out += uniform_traits<U>::size( element );

        return out;
    }

    static GLenum type() { return uniform_traits<U>::type(); }
};

} // namespace ugl
#endif // _UNIFORMTRAITS_HPP_
