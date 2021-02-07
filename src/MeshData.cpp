/** @file MeshData.cpp



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

#include "ugl/MeshData.hpp"

#include <algorithm>
#include <set>
#include <utility>

namespace ugl
{

void MeshData::recomputeVertexNormals()
{
    m_vertexNormals.assign(getPointCount(), glm::vec3(0.0f));

    for (GLint triangleId = 0; triangleId < getTriangleCount(); ++triangleId)
    {
        const glm::uvec3& triangle = getTriangle(triangleId);

        const glm::vec3& p0 = getPoint(triangle[0u]);
        const glm::vec3& p1 = getPoint(triangle[1u]);
        const glm::vec3& p2 = getPoint(triangle[2u]);

        glm::vec3 normal = glm::cross(p1 - p0, p2 - p0);

        m_vertexNormals[triangle[0u]] += normal;
        m_vertexNormals[triangle[1u]] += normal;
        m_vertexNormals[triangle[2u]] += normal;
    }

    for (size_t pointId = 0u; pointId < m_vertexNormals.size(); ++pointId)
    {
        if (glm::length(m_vertexNormals[pointId]) > 0.0f)
            m_vertexNormals[pointId] = glm::normalize(m_vertexNormals[pointId]);
    }

    m_vertexNormalsValid = true;
}

// -------------------------------------------------------------------------

void MeshData::recomputeEdges()
{
    // first, count the edges emanating from each vertex
    // (to avoid counting opposite edges twice, an edge is always taken
    // to emanate from the vertex with the smaller index)
    std::set< std::pair<GLuint, GLuint> > edgeSet;

    for (GLint triangleId = 0; triangleId < getTriangleCount(); ++triangleId)
    {
        const glm::uvec3& triangle = getTriangle(triangleId);
        const GLuint i0 = triangle[0u], i1 = triangle[1u], i2 = triangle[2u];

        // 1st edge: 0 -> 1
        edgeSet.emplace(std::make_pair(std::min(i0, i1), std::max(i0, i1)));
        // 2nd edge: 1 -> 2
        edgeSet.emplace(std::make_pair(std::min(i1, i2), std::max(i1, i2)));
        // 3rd edge: 2 -> 0
        edgeSet.emplace(std::make_pair(std::min(i2, i0), std::max(i2, i0)));
    }

    // copy edges from the set to a linear array
    m_edges.clear();
    m_edges.reserve(edgeSet.size());
    for (std::set< std::pair<GLuint, GLuint> >::const_iterator edgePair = edgeSet.begin();
            edgePair != edgeSet.end(); ++edgePair)
        m_edges.emplace_back(glm::uvec2(edgePair->first, edgePair->second));

    m_edgesValid = true;
}


} // namespace ugl
