#ifndef __surfacevectordata_hpp
#define __surfacevectordata_hpp

#include <string>
#include <vector>

#include <glm/glm.hpp>

/**
 * Data class which loads a surface and vector values at its vertices from a
 * NetCDF file.
 */
class SurfaceVectorData
{
public:

    SurfaceVectorData( const char* filename );
    virtual ~SurfaceVectorData();

    /// query the number of triangles defining the surface
    unsigned int getNumberOfTriangles() const;

    /// get the i-th triangles point indices
    const glm::uvec3& getTriangleIndices( unsigned int i ) const;

    /// query the number of points
    unsigned int getNumberOfPoints() const;

    /// get the i-th point's coordinates
    const glm::dvec3& getPoint( unsigned int i ) const;

    /// get the i-th point's vector
    const glm::dvec3& getVector( unsigned int i ) const;

protected:

    std::vector<glm::dvec3>  m_points;
    std::vector<glm::dvec3>  m_vectors;
    std::vector<glm::uvec3>  m_triangles;

private:

    /// disallow copying to avoid memory management problems
    SurfaceVectorData( const SurfaceVectorData& );
};

#endif // __surfacevectordata_hpp
