project( ugl-prototype )
cmake_minimum_required( VERSION 2.8.9 )

# Policies
if( POLICY CMP0020 )
    cmake_policy( SET CMP0020 OLD )
endif()
if( POLICY CMP0043 )
    cmake_policy( SET CMP0043 OLD )
endif()
if( POLICY CMP0042 )
    cmake_policy( SET CMP0042 NEW )
endif()


# Load custom CMake modules
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )


# Set compiler flags.
if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
    set( WARNING_LEVEL 2 CACHE STRING
        "0 inhibit warnings; 1 no changes; 2 output warnings (default); 3 treat warnings as errors" )

    if( WARNING_LEVEL EQUAL 3 )
        set( WARNING_COMPILE_FLAGS "-pedantic-errors -Wall -Wextra -Werror -Wfatal-errors" )
    elseif( WARNING_LEVEL EQUAL 2 )
        set( WARNING_COMPILE_FLAGS "-pedantic -Wall -Wextra" )
    elseif( WARNING_LEVEL EQUAL 1 )
        set( WARNING_COMPILE_FLAGS "" )
    elseif( WARNING_LEVEL EQUAL 0 )
        set( WARNING_COMPILE_FLAGS "-w" )
    endif()

    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${WARNING_COMPILE_FLAGS}" )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
    # Disable warnings on using functions of the C++ Standard Library.
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

# Compilation options
option( UGL_BUILD_QT "Compile ugl-qt module based on Qt." OFF )
option( UGL_BUILD_WXWIDGETS "Compile ugl-wx module based on wxWidgets." OFF )
option( UGL_BUILD_SDL "Compile ugl-sdl module based on SDL 2." OFF )
option( UGL_BUILD_EXAMPLES "Compile example applications." ON )
option( BUILD_SHARED_LIBS "Build shared libs." ON )

find_package( Boost COMPONENTS system filesystem REQUIRED )
find_package(GLM REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIRS} 
    ${GLM_INCLUDE_DIRS}
    )

# Module: ugl library
add_subdirectory( src )



# Module: examples
if ( UGL_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()


