# Libraries required for examples
find_package( AntTweakBar REQUIRED )
include_directories( ${ANTTWEAKBAR_INCLUDE_DIRS} )

find_package( NetCDF REQUIRED )
include_directories( ${NETCDF_INCLUDE_DIRS} )

find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_DIRS} )

find_package( GLM REQUIRED )
include_directories( ${GLM_INCLUDE_DIRS} )


# ----------------------------------------------------
# Build Qt examples
# ----------------------------------------------------
if ( UGL_BUILD_QT )
    # Add Qt 5 modules
    find_package( Qt5Widgets REQUIRED )
    find_package( Qt5OpenGL REQUIRED )
    find_package( Qt5Gui REQUIRED )
    find_package( Qt5Core REQUIRED )
    find_package( GLEW REQUIRED )

    # Include paths
    include_directories(
        ..
        ../libs
        ../include
        ${Qt5OpenGL_INCLUDE_DIRS}
    )

    # basic_viewer_qt
    add_executable( basic_viewer_qt basic_viewer/BasicViewer.cpp basic_viewer/main-qt.cpp )
    target_link_libraries( basic_viewer_qt ugl ugl-qt Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets ${ANTTWEAKBAR_LIBRARY})

    # viewer_qt
    add_executable( viewer_qt viewer/Viewer.cpp viewer/SurfaceRenderer.cpp viewer/SurfaceVectorData.cpp viewer/SkullVolume.cpp viewer/main-qt.cpp )
    target_link_libraries( viewer_qt ugl ugl-qt Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets ${ANTTWEAKBAR_LIBRARY} ${NETCDF_LIBRARIES})

    # render_to_texture_qt
    add_executable( render_to_texture_qt render_to_texture/RenderToTexture.cpp render_to_texture/RedTriangle.cpp render_to_texture/main-qt.cpp )
    target_link_libraries( render_to_texture_qt ugl ugl-qt Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets)

    # dynamic_fragment_buffer_test
    add_executable( dynamic_fragment_buffer_test_qt dynamic_fragment_buffer_test/DFBTest.cpp dynamic_fragment_buffer_test/SurfaceVectorData.cpp dynamic_fragment_buffer_test/SurfaceRenderer.cpp dynamic_fragment_buffer_test/main-qt.cpp )
    target_link_libraries( dynamic_fragment_buffer_test_qt ugl ugl-qt Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets ${ANTTWEAKBAR_LIBRARY} ${NETCDF_LIBRARIES})

endif()



# ----------------------------------------------------
# Build wxWidgets examples
# ----------------------------------------------------
if ( UGL_BUILD_WXWIDGETS)
    # Add wxWidgets modules
    find_package( wxWidgets REQUIRED gl core base )
    include( ${wxWidgets_USE_FILE} )

    # Include paths
    include_directories(
        ..
        ../libs
        ../include
        ${wxWidgets_INCLUDE_DIRS}
    )

    # basic_viewer_wx
    add_executable( basic_viewer_wx basic_viewer/BasicViewer.cpp basic_viewer/main-wx.cpp )
    target_link_libraries( basic_viewer_wx ugl ugl-wx ${wxWidgets_LIBRARIES} ${ANTTWEAKBAR_LIBRARY})

    # viewer_wx
    add_executable( viewer_wx viewer/Viewer.cpp viewer/SurfaceRenderer.cpp viewer/SurfaceVectorData.cpp viewer/SkullVolume.cpp viewer/main-wx.cpp )
    target_link_libraries( viewer_wx ugl ugl-wx ${wxWidgets_LIBRARIES} ${ANTTWEAKBAR_LIBRARY} ${NETCDF_LIBRARIES})

    # render_to_texture_wx
    add_executable( render_to_texture_wx render_to_texture/RenderToTexture.cpp render_to_texture/RedTriangle.cpp render_to_texture/main-wx.cpp )
    target_link_libraries( render_to_texture_wx ugl ugl-wx ${wxWidgets_LIBRARIES} )

    # Seems to be needed for Visual Studio to find the main() function of wxWidgets apps.
    if( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
        set_target_properties( basic_viewer_wx PROPERTIES LINK_FLAGS "-SUBSYSTEM:WINDOWS" )
        set_target_properties( viewer_wx PROPERTIES LINK_FLAGS "-SUBSYSTEM:WINDOWS" )
        set_target_properties( render_to_texture_wx PROPERTIES LINK_FLAGS "-SUBSYSTEM:WINDOWS" )
    endif()
endif()


# ----------------------------------------------------
# Build SDL examples
# ----------------------------------------------------
if ( UGL_BUILD_SDL)
    # Add SDL2 modules
    find_package(SDL2 REQUIRED)

    # Include paths
    include_directories(
        ..
        ../libs
        ../include
        ${SDL2_INCLUDE_DIRS}
    )

    # basic_viewer_sdl
    add_executable( basic_viewer_sdl basic_viewer/BasicViewer.cpp basic_viewer/main-sdl.cpp )
    target_link_libraries( basic_viewer_sdl ugl ugl-sdl ${SDL2_LIBRARIES} ${ANTTWEAKBAR_LIBRARY})

    # viewer_sdl
    add_executable( viewer_sdl viewer/Viewer.cpp viewer/SurfaceRenderer.cpp viewer/SurfaceVectorData.cpp viewer/SkullVolume.cpp viewer/main-sdl.cpp )
    target_link_libraries( viewer_sdl ugl ugl-sdl ${SDL2_LIBRARIES} ${ANTTWEAKBAR_LIBRARY} ${NETCDF_LIBRARIES})

    # render_to_texture_sdl
    add_executable( render_to_texture_sdl render_to_texture/RenderToTexture.cpp render_to_texture/RedTriangle.cpp render_to_texture/main-sdl.cpp )
    target_link_libraries( render_to_texture_sdl ugl ugl-sdl ${SDL2_LIBRARIES})

    # dynamic_fragment_buffer_test_sdl
    add_executable( dynamic_fragment_buffer_test_sdl dynamic_fragment_buffer_test/DFBTest.cpp dynamic_fragment_buffer_test/SurfaceVectorData.cpp dynamic_fragment_buffer_test/SurfaceRenderer.cpp dynamic_fragment_buffer_test/main-sdl.cpp )
    target_link_libraries( dynamic_fragment_buffer_test_sdl ugl ugl-sdl ${SDL2_LIBRARIES} ${ANTTWEAKBAR_LIBRARY} ${NETCDF_LIBRARIES})

    # afront_viwer_sdl
    add_executable( afront_viewer_sdl afront_viewer/AfrontViewer.cpp afront_viewer/StreamingMesh.cpp afront_viewer/main-sdl.cpp )
    target_link_libraries( afront_viewer_sdl ugl ugl-sdl ${SDL2_LIBRARIES} ${ANTTWEAKBAR_LIBRARY})
endif()
