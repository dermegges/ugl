# - Try to find GLEW
# Once done this will define
#  GLEW_FOUND - System has GLEW
#  GLEW_INCLUDE_DIRS - The GLEW include directories
#  GLEW_LIBRARIES - The libraries needed to use GLEW
#  GLEW_DEFINITIONS - Compiler switches required for using GLEW

find_path( GLEW_INCLUDE_DIR glew.h PATH_SUFFIXES GL )
find_library( GLEW_LIBRARY NAMES GLEW )

set( GLEW_LIBRARIES ${GLEW_LIBRARY} )
set( GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR} )

include( FindPackageHandleStandardArgs )

# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( GLEW  DEFAULT_MSG
                                   GLEW_LIBRARY GLEW_INCLUDE_DIR )

mark_as_advanced( GLEW_INCLUDE_DIR GLEW_LIBRARY )
