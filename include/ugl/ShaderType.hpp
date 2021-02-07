/** @file ShaderType.hpp



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

#ifndef _SHADERTYPE_HPP_
#define _SHADERTYPE_HPP_

#include <GL/glew.h>

namespace ugl
{

/**
 * Enumeration of OpenGL shader types including COMBINED which will be split
 * into the correct shader stages by SourceSplitter.
 */
enum ShaderType : GLenum
{
    VERTEX   		= GL_VERTEX_SHADER,
    FRAGMENT 		= GL_FRAGMENT_SHADER,
    GEOMETRY 		= GL_GEOMETRY_SHADER,
    TESS_CONTROL 	= GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION	= GL_TESS_EVALUATION_SHADER,

	COMPUTE			= GL_COMPUTE_SHADER,

    COMBINED = GL_VERTEX_SHADER + GL_FRAGMENT_SHADER + GL_GEOMETRY_SHADER
};

} // namespace ugl
#endif // _SHADERTYPE_HPP_
