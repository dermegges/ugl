#include "SurfaceVectorData.hpp"

#include <netcdf.h>

#include <iostream>
#include <stdexcept>

using namespace std;

struct netcdf_exception: public std::exception
{
    int error;

    netcdf_exception( int e ) : error(e)
    {
    }

    const char* what() const throw()
    {
        return nc_strerror(error);
    }
};

static void nccheck( int result )
{
    if( result != NC_NOERR )
        throw netcdf_exception( result );
}

// -------------------------------------------------------------------------

SurfaceVectorData::SurfaceVectorData( const char* filename )
{
    string gridfile = string(filename) + ".grid";
    string pvalfile = string(filename) + ".pval";

    int ncid;

    try
    {
        size_t npoints = 0, ntri = 0;
        nccheck( nc_open( gridfile.c_str(), NC_NOWRITE, &ncid ) );

        int pdimid;
        nccheck( nc_inq_dimid( ncid, "no_of_points", &pdimid ) );
        nccheck( nc_inq_dimlen( ncid, pdimid, &npoints ) );

        int cdimid;

        if( NC_NOERR == nc_inq_dimid( ncid, "no_of_surfacetriangles", &cdimid ) )
        {
            nccheck( nc_inq_dimlen( ncid, cdimid, &ntri ) );
        }

        std::cerr << "loaded surface with "
                  << npoints << " points and "
                  << ntri << " triangles" << std::endl;

        // ---

        m_points.resize( npoints );
        double* ftmp = new double[npoints];

        try
        {
            int varid;

            nccheck( nc_inq_varid( ncid, "points_xc", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<npoints; ++i )
                m_points[i].x = ftmp[i];

            nccheck( nc_inq_varid( ncid, "points_yc", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<npoints; ++i )
                m_points[i].y = ftmp[i];

            nccheck( nc_inq_varid( ncid, "points_zc", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<npoints; ++i )
                m_points[i].z = ftmp[i];

            delete[] ftmp;
        }
        catch( ... )
        {
            delete[] ftmp;
            throw;
        }

        // --- read triangles

        m_triangles.resize( ntri );
        int *itmp = new int[3*ntri];

        try
        {
            int varid, dimid[2];
            size_t dimsz[2];

            if( NC_NOERR == nc_inq_varid( ncid, "points_of_surfacetriangles", &varid ) )
            {
                nccheck( nc_inq_vardimid( ncid, varid, dimid ) );
                nccheck( nc_inq_dimlen( ncid, dimid[0], &dimsz[0] ) );
                nccheck( nc_inq_dimlen( ncid, dimid[1], &dimsz[1] ) );

                nccheck( nc_get_var_int( ncid, varid, itmp ) );

                for( unsigned int i=0; i<ntri; ++i )
                    m_triangles[i] = glm::uvec3( itmp[3*i+0], itmp[3*i+1], itmp[3*i+2] );
            }

            delete[] itmp;
        }
        catch( ... )
        {
            delete[] itmp;
            throw;
        }

        nc_close( ncid );
    }
    catch( ... )
    {
        nc_close( ncid );
    }

    // --- read vectors

    try
    {
        size_t nvectors;

        nccheck( nc_open( pvalfile.c_str(), NC_NOWRITE, &ncid ) );

        int pdimid;
        nccheck( nc_inq_dimid( ncid, "no_of_points", &pdimid ) );
        nccheck( nc_inq_dimlen( ncid, pdimid, &nvectors ) );

        m_vectors.resize( nvectors );
        double* ftmp = new double[nvectors];

        try
        {
            int varid;

            nccheck( nc_inq_varid( ncid, "x_vector", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<nvectors; ++i )
                m_vectors[i].x = ftmp[i];

            nccheck( nc_inq_varid( ncid, "y_vector", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<nvectors; ++i )
                m_vectors[i].y = ftmp[i];

            nccheck( nc_inq_varid( ncid, "z_vector", &varid ) );
            nccheck( nc_get_var_double( ncid, varid, ftmp ) );

            for( unsigned int i=0; i<nvectors; ++i )
                m_vectors[i].z = ftmp[i];

            delete[] ftmp;
        }
        catch( ... )
        {
            delete[] ftmp;
            throw;
        }

        nc_close( ncid );
    }
    catch( ... )
    {
        nc_close( ncid );
        throw;
    }
}

// -------------------------------------------------------------------------

SurfaceVectorData::~SurfaceVectorData()
{
}

// -------------------------------------------------------------------------

unsigned int SurfaceVectorData::getNumberOfTriangles() const
{
    return m_triangles.size();
}

// -------------------------------------------------------------------------

const glm::uvec3& SurfaceVectorData::getTriangleIndices( unsigned int i ) const
{
    return m_triangles[i];
}

// -------------------------------------------------------------------------

unsigned int SurfaceVectorData::getNumberOfPoints() const
{
    return m_points.size();
}

// -------------------------------------------------------------------------

const glm::dvec3& SurfaceVectorData::getPoint( unsigned int i ) const
{
    return m_points[i];
}

// -------------------------------------------------------------------------

const glm::dvec3& SurfaceVectorData::getVector( unsigned int i ) const
{
    return m_vectors[i];
}

// -------------------------------------------------------------------------
