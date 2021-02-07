# ugl source files
set( UGL_SOURCE_FILES
    BoundingBox.cpp
    CoreApplication.cpp
    CurvesCsvLoader.cpp
    CurvesData.cpp
    CurvesDrawable.cpp
    DefaultRenderStage.cpp
    Drawable.cpp
    ErrorCheck.cpp
    FileSystemWatcher.cpp
    Framebuffer.cpp
    GLSLPreprocessor.cpp
    MeshData.cpp
    MeshDrawable.cpp
    ScalarData.cpp
    ScalarValues.cpp
    ShaderProgram.cpp
    SourceSplitter.cpp
    StateSet.cpp
    TransparentRenderStage.cpp
    TransparentRenderStageDFB.cpp
    UniformSet.cpp
    Utils.cpp
    VariantProgram.cpp
    Volume.cpp
    ViewController3D.cpp

    # FileWatcher library
    ../libs/FileWatcher/FileWatcher.cpp
    ../libs/FileWatcher/FileWatcher.h
    ../libs/FileWatcher/FileWatcherImpl.h
    ../libs/FileWatcher/FileWatcherLinux.cpp
    ../libs/FileWatcher/FileWatcherLinux.h
    ../libs/FileWatcher/FileWatcherOSX.cpp
    ../libs/FileWatcher/FileWatcherOSX.h
    ../libs/FileWatcher/FileWatcherWin32.cpp
    ../libs/FileWatcher/FileWatcherWin32.h


    # These are not required for compilation... but force QtCreator to show them in the files tree.
    ../include/ugl/AbstractValueSet.hpp
    ../include/ugl/BoundingBox.hpp
    ../include/ugl/CoreApplication.hpp
    ../include/ugl/CurvesCsvLoader.hpp
    ../include/ugl/CurvesData.hpp
    ../include/ugl/CurvesDrawable.hpp
    ../include/ugl/DefaultRenderStage.hpp
    ../include/ugl/Drawable.hpp
    ../include/ugl/ErrorCheck.hpp
    ../include/ugl/FileSystemWatcher.hpp
    ../include/ugl/Framebuffer.hpp
    ../include/ugl/GLHelper.hpp
    ../include/ugl/GLSLPreprocessor.hpp
    ../include/ugl/MeshData.hpp
    ../include/ugl/MeshDrawable.hpp
    ../include/ugl/ModeSet.hpp
    ../include/ugl/NoValues.hpp
    ../include/ugl/ScalarData.hpp
    ../include/ugl/ScalarValues.hpp
    ../include/ugl/ShaderProgram.hpp
    ../include/ugl/ShaderType.hpp
    ../include/ugl/SourceSplitter.hpp
    ../include/ugl/StateSet.hpp
    ../include/ugl/TransparentRenderStage.hpp
    ../include/ugl/TransparentRenderStageDFB.hpp
    ../include/ugl/UniformSet.hpp
    ../include/ugl/UniformTraits.hpp
    ../include/ugl/Utils.hpp
    ../include/ugl/Values.hpp
    ../include/ugl/VariantProgram.hpp
    ../include/ugl/VecConv.hpp
    ../include/ugl/Volume.hpp
    ../include/ugl/ViewController.hpp
    ../include/ugl/ViewController3D.hpp
)


# Libraries
find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLM REQUIRED ) 

include_directories(
    ../include
    ../libs
    SYSTEM
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)


# Module: ugl
add_library( ugl ${UGL_SOURCE_FILES} )
target_link_libraries( ugl ${GLEW_LIBRARY} ${OPENGL_gl_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})
set_target_properties(ugl PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)


# Module: ugl-qt
if ( UGL_BUILD_QT )
    # Add Qt 5 modules
    find_package( Qt5Widgets REQUIRED )
    find_package( Qt5OpenGL REQUIRED )
    find_package( Qt5Gui REQUIRED )
    find_package( Qt5Core REQUIRED )

    # Include paths
    include_directories(
        ${Qt5OpenGL_INCLUDE_DIRS}
    )

    # Enable moc
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)

    # Create library
    add_library(ugl-qt qt/Widget.cpp ../include/ugl/qt/Widget.hpp qt/OpenGLWidget.cpp ../include/ugl/qt/OpenGLWidget.hpp)
    #target_link_libraries(ugl-qt ugl Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets)
     target_link_libraries(ugl-qt ugl Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Widgets GL GLU glut)
    set_target_properties(ugl-qt PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()


# Module: ugl-wx
if ( UGL_BUILD_WXWIDGETS )
    find_package(wxWidgets COMPONENTS core base gl REQUIRED)
    include(${wxWidgets_USE_FILE})

    include_directories(
        ${wxWidgets_INCLUDE_DIRS}
    )

    # Create library
    add_library(ugl-wx wx/Canvas.cpp ../include/ugl/wx/Canvas.hpp)
    target_link_libraries(ugl-wx ugl ${wxWidgets_LIBRARIES})
    set_target_properties(ugl-wx PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()


# Module: ugl-sdl
if ( UGL_BUILD_SDL )
    find_package(SDL2 REQUIRED SDL2_BUILDING_LIBRARY)

    include_directories(
        ${SDL2_INCLUDE_DIRS}
    )

    # Create library
    add_library(ugl-sdl sdl/App.cpp ../include/ugl/sdl/App.hpp)
    target_link_libraries(ugl-sdl ugl ${SDL2_LIBRARIES})
    set_target_properties(ugl-sdl PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
endif()
