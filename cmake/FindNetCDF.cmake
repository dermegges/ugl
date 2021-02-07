# - Try to find NetCDF
# Once done this will define
#  NETCDF_FOUND - System has GLEW
#  NETCDF_INCLUDE_DIRS - The GLEW include directories
#  NETCDF_LIBRARIES - The libraries needed to use GLEW
#  NETCDF_DEFINITIONS - Compiler switches required for using GLEW

find_path( NETCDF_INCLUDE_DIR netcdf.h )
find_library( NETCDF_LIBRARY NAMES netcdf )

set( NETCDF_LIBRARIES ${NETCDF_LIBRARY} )
set( NETCDF_INCLUDE_DIRS ${NETCDF_INCLUDE_DIR} )

include( FindPackageHandleStandardArgs )

# handle the QUIETLY and REQUIRED arguments and set NETCDF_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( NetCDF DEFAULT_MSG
                                   NETCDF_LIBRARY NETCDF_INCLUDE_DIR )

mark_as_advanced( NETCDF_INCLUDE_DIR NETCDF_LIBRARY )
