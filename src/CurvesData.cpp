/** @file CurvesData.cpp



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

#include "ugl/CurvesData.hpp"

namespace ugl
{

void CurvesData::addCurve(const std::vector<glm::vec3>& curve)
{
    m_startPointIds.push_back(m_points.size());
    m_pointCounts.push_back(curve.size());
    m_points.insert(m_points.end(), curve.cbegin(), curve.cend());
}

// -------------------------------------------------------------------------

void CurvesData::addEmptyCurve()
{
    m_startPointIds.push_back(m_points.size());
    m_pointCounts.push_back(0);
}

// -------------------------------------------------------------------------

void CurvesData::addPointToLastCurve(const glm::vec3& point)
{
    ++m_pointCounts.back();
    m_points.push_back(point);
}

// -------------------------------------------------------------------------

void CurvesData::recomputeCurveIdsOfPointIds()
{
    m_curveIdsOfPointIds.clear();
    m_curveIdsOfPointIds.reserve(getTotalPointCount());

    for (GLint curveId = 0; curveId < getCurveCount(); ++curveId)
    {
        m_curveIdsOfPointIds.insert(m_curveIdsOfPointIds.end(),
                                    getPointCount(curveId), curveId);
    }

    m_curveIdsOfPointIdsValid = true;
}

} // namespace ugl
