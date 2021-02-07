/** @file GLHelper.hpp



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

#ifndef _GLHELPER_HPP_
#define _GLHELPER_HPP_

#include <GL/glew.h>

#include <vector>

namespace ugl
{

/**
 * A collection of utility routines useful for working with OpenGL.
 *
 * Currently just one helper function which copies a STL-vector into an
 * OpenGL-Buffer.
 */
class GLHelper
{
public:
    /**
     * Helper function for repeated buffer creation
     */
    template <typename T>
    static GLuint prepareStaticBuffer(const std::vector<T>& data);
};

// -------------------------------------------------------------------------

template <typename T>
GLuint GLHelper::prepareStaticBuffer(const std::vector<T>& data)
{
    GLuint buffer;
    glGenBuffers( 1, &buffer );

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0],
                  GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );

    return buffer;
}

} // namespace ugl
#endif // _GLHELPER_HPP_
