/** @file VecConv.hpp



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

#ifndef _VECCONV_HPP_
#define _VECCONV_HPP_

#include <glm/glm.hpp>
#include <QVector3D>

namespace ugl
{

/**
 * Utility functions to convert between GLM and Qt vectors.
 */
class VecConv
{
public:
    static QVector3D toQVector3D(const glm::vec3& glm);
};

// -------------------------------------------------------------------------

inline QVector3D VecConv::toQVector3D(const glm::vec3& glm)
{
    return QVector3D(glm[0u], glm[1u], glm[2u]);
}

} // namespace ugl
#endif // _VECCONV_HPP_
