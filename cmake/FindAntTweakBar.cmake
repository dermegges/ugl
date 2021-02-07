# - Try to find AntTweakBar
# Once done this will define
#  ANTTWEAKBAR_FOUND - System has AntTweakBar
#  ANTTWEAKBAR_INCLUDE_DIRS - The AntTweakBar include directories
#  ANTTWEAKBAR_LIBRARIES - The libraries needed to use AntTweakBar
#  ANTTWEAKBAR_DEFINITIONS - Compiler switches required for using AntTweakBar

find_path( ANTTWEAKBAR_INCLUDE_DIR AntTweakBar.h PATHS /home/uni-kl.de/lohfink/Documents/AntTweakBar/include )

find_library( ANTTWEAKBAR_LIBRARY NAMES AntTweakBar PATHS /home/uni-kl.de/lohfink/Documents/AntTweakBar/lib )

set( ANTTWEAKBAR_LIBRARIES ${ANTTWEAKBAR_LIBRARY} )
set( ANTTWEAKBAR_INCLUDE_DIRS ${ANTTWEAKBAR_INCLUDE_DIR} )

include( FindPackageHandleStandardArgs )

# handle the QUIETLY and REQUIRED arguments and set ANTTWEAKBAR_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( AntTweakBar  DEFAULT_MSG
                                   ANTTWEAKBAR_LIBRARY ANTTWEAKBAR_INCLUDE_DIR )

mark_as_advanced( ANTTWEAKBAR_INCLUDE_DIR ANTTWEAKBAR_LIBRARY )
