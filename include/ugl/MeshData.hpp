/** @file MeshData.hpp



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

#ifndef _MESHDATA_HPP_
#define _MESHDATA_HPP_

#include "BoundingBox.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <vector>

namespace ugl
{

/**
 * Data class for a mesh made up of triangles using indices to a list of points.
 *
 * Vertex normals and edges can be computed.
 */
class MeshData
{
public:
    MeshData();

    /// query the number of points
    GLsizei getPointCount() const;

    /// get the i-th point's coordinates
    const glm::vec3& getPoint(GLint pointId) const;

    /// get the whole vector of all points
    const std::vector<glm::vec3>& getPoints() const;

    size_t addPoint(const glm::vec3& point);

    void setPoints(const std::vector<glm::vec3>& points);

    /// query the number of triangles defining the mesh
    GLsizei getTriangleCount() const;

    /// get the i-th triangle's point indices
    const glm::uvec3& getTriangle(GLint triangleId) const;

    /// get the whole vector of all triangles
    const std::vector<glm::uvec3>& getTriangles() const;

    void addTriangle(const glm::uvec3& triangle);

    void setTriangles(const std::vector<glm::uvec3>& triangles);

    const glm::vec3& getVertexNormal(GLint pointId);
    const std::vector<glm::vec3>& getVertexNormals();
    void recomputeVertexNormals();

    const BoundingBox& getBoundingBox();
    void recomputeBoundingBox();

    GLsizei getEdgeCount();
    const glm::uvec2& getEdge(GLint edgeId);
    const std::vector<glm::uvec2>& getEdges();
    void recomputeEdges();

private:
    std::vector<glm::vec3>  m_points;
    std::vector<glm::uvec3> m_triangles;

    bool                    m_vertexNormalsValid;
    std::vector<glm::vec3>  m_vertexNormals;

    bool                    m_boundingBoxValid;
    BoundingBox             m_boundingBox;

    bool                    m_edgesValid;
    std::vector<glm::uvec2> m_edges;
};

// -------------------------------------------------------------------------

inline MeshData::MeshData() :
    m_vertexNormalsValid(false),
    m_boundingBoxValid(false),
    m_edgesValid(false)
{
}

// -------------------------------------------------------------------------

inline GLsizei MeshData::getPointCount() const
{
    return m_points.size();
}

// -------------------------------------------------------------------------

inline const glm::vec3& MeshData::getPoint(GLint pointId) const
{
    return m_points[pointId];
}

// -------------------------------------------------------------------------

inline const std::vector<glm::vec3>& MeshData::getPoints() const
{
    return m_points;
}

// -------------------------------------------------------------------------

inline size_t MeshData::addPoint(const glm::vec3& point)
{
    m_points.push_back(point);
    return m_points.size() - 1;
}

// -------------------------------------------------------------------------

inline void MeshData::setPoints(const std::vector<glm::vec3>& points)
{
    m_points = points;
}

// -------------------------------------------------------------------------

inline GLsizei MeshData::getTriangleCount() const
{
    return m_triangles.size();
}

// -------------------------------------------------------------------------

inline const glm::uvec3& MeshData::getTriangle(GLint triangleId) const
{
    return m_triangles[triangleId];
}

// -------------------------------------------------------------------------

inline const std::vector<glm::uvec3>& MeshData::getTriangles() const
{
    return m_triangles;
}

// -------------------------------------------------------------------------

inline void MeshData::addTriangle(const glm::uvec3& triangle)
{
    m_triangles.push_back(triangle);
}

// -------------------------------------------------------------------------

inline void MeshData::setTriangles(const std::vector<glm::uvec3>& triangles)
{
    m_triangles = triangles;
}

// -------------------------------------------------------------------------

inline const glm::vec3& MeshData::getVertexNormal(GLint pointId)
{
    if (!m_vertexNormalsValid)
        recomputeVertexNormals();

    return m_vertexNormals[pointId];
}

// -------------------------------------------------------------------------

inline const std::vector<glm::vec3>& MeshData::getVertexNormals()
{
    if (!m_vertexNormalsValid)
        recomputeVertexNormals();

    return m_vertexNormals;
}

// -------------------------------------------------------------------------

inline const BoundingBox& MeshData::getBoundingBox()
{
    if (!m_boundingBoxValid)
        recomputeBoundingBox();

    return m_boundingBox;
}

// -------------------------------------------------------------------------

inline void MeshData::recomputeBoundingBox()
{
    m_boundingBox.computeFromPositions(m_points);
    m_boundingBoxValid = true;
}

// -------------------------------------------------------------------------

inline GLsizei MeshData::getEdgeCount()
{
    if (!m_edgesValid)
        recomputeEdges();

    return m_edges.size();
}

// -------------------------------------------------------------------------

inline const glm::uvec2& MeshData::getEdge(GLint edgeId)
{
    if (!m_edgesValid)
        recomputeEdges();

    return m_edges[edgeId];
}

// -------------------------------------------------------------------------

inline const std::vector<glm::uvec2>& MeshData::getEdges()
{
    if (!m_edgesValid)
        recomputeEdges();

    return m_edges;
}

} // namespace ugl
#endif // _MESHDATA_HPP_
