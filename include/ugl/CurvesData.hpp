/** @file CurvesData.hpp



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

#ifndef _CURVESDATA_HPP_
#define _CURVESDATA_HPP_

#include "BoundingBox.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vector>

namespace ugl
{

/**
 * Data class for multiple curves of different lengths.
 */
class CurvesData
{
public:
    CurvesData();

    GLsizei                         getCurveCount() const;
    GLsizei                         getPointCount(GLint curveId) const;
    const glm::vec3&                getPoint(GLint curveId,
                                             GLint relativePointId) const;

    const std::vector<GLsizei>&     getAllPointCounts() const;
    GLsizei                         getTotalPointCount() const;
    const std::vector<glm::vec3>&   getAllPoints() const;
    const std::vector<GLint>&       getAllStartPointIds() const;

    void addCurve(const std::vector<glm::vec3>& curve);
    void addEmptyCurve();
    void addPointToLastCurve(const glm::vec3& point);

    const BoundingBox&              getBoundingBox();
    void                            recomputeBoundingBox();

    GLint                           getCurveIdOfPointId(GLint pointId);
    const std::vector<GLint>&       getAllCurveIdsOfPointIds();
    void                            recomputeCurveIdsOfPointIds();

private:
    std::vector<glm::vec3>  m_points;
    std::vector<GLint>      m_startPointIds;
    std::vector<GLsizei>    m_pointCounts;

    bool                    m_boundingBoxValid;
    BoundingBox             m_boundingBox;

    bool                    m_curveIdsOfPointIdsValid;
    std::vector<GLint>      m_curveIdsOfPointIds;
};

// -------------------------------------------------------------------------

inline CurvesData::CurvesData() :
    m_boundingBoxValid(false),
    m_curveIdsOfPointIdsValid(false)
{
}

// -------------------------------------------------------------------------

inline GLsizei CurvesData::getCurveCount() const
{
    return m_startPointIds.size();
}

// -------------------------------------------------------------------------

inline GLsizei CurvesData::getPointCount(GLint curveId) const
{
    return m_pointCounts[curveId];
}

// -------------------------------------------------------------------------

inline const glm::vec3& CurvesData::getPoint(GLint curveId,
                                             GLint relativePointId) const
{
    return m_points[m_startPointIds[curveId] + relativePointId];
}

// -------------------------------------------------------------------------

inline const std::vector<GLsizei>& CurvesData::getAllPointCounts() const
{
    return m_pointCounts;
}

// -------------------------------------------------------------------------

inline GLsizei CurvesData::getTotalPointCount() const
{
    return m_points.size();
}

// -------------------------------------------------------------------------

inline const std::vector<glm::vec3>& CurvesData::getAllPoints() const
{
    return m_points;
}

// -------------------------------------------------------------------------

inline const std::vector<GLint>& CurvesData::getAllStartPointIds() const
{
    return m_startPointIds;
}

// -------------------------------------------------------------------------

inline const BoundingBox& CurvesData::getBoundingBox()
{
    if (!m_boundingBoxValid)
        recomputeBoundingBox();

    return m_boundingBox;
}

// -------------------------------------------------------------------------

inline void CurvesData::recomputeBoundingBox()
{
    m_boundingBox.computeFromPositions(m_points);
    m_boundingBoxValid = true;
}

// -------------------------------------------------------------------------

inline GLint CurvesData::getCurveIdOfPointId(GLint pointId)
{
    if (!m_curveIdsOfPointIdsValid)
        recomputeCurveIdsOfPointIds();

    return m_curveIdsOfPointIds[pointId];
}

// -------------------------------------------------------------------------

inline const std::vector<GLint>& CurvesData::getAllCurveIdsOfPointIds()
{
    if (!m_curveIdsOfPointIdsValid)
        recomputeCurveIdsOfPointIds();

    return m_curveIdsOfPointIds;
}

} // namespace ugl
#endif // _CURVESDATA_HPP_
