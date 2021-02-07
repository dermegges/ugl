/** @file CurvesCsvLoader.cpp



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

#include "ugl/CurvesCsvLoader.hpp"

#include <glm/glm.hpp>

#include <fstream>

namespace ugl
{

void CurvesCsvLoader::addCurve(const std::string& filename)
{
    std::ifstream in(filename);

    unsigned pointCount;
    in >> pointCount;

    m_curvesData.addEmptyCurve();

    float     time;
    glm::vec3 point;
    for (unsigned i = 0u; i < pointCount; ++i)
    {
        in >> time >> point.x >> point.y >> point.z;
        m_curvesData.addPointToLastCurve(point);
        m_scalarData.addScalar(time);
    }
}

} // namespace ugl
