# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/markus/Downloads/ugl-prototype-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/markus/Downloads/ugl-prototype-master/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/viewer_qt.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/viewer_qt.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/viewer_qt.dir/flags.make

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o: examples/CMakeFiles/viewer_qt.dir/flags.make
examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o: ../examples/viewer/Viewer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/viewer/Viewer.cpp

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/viewer/Viewer.cpp > CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.i

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/viewer/Viewer.cpp -o CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.s

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.requires:

.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.requires

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.provides: examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/viewer_qt.dir/build.make examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.provides.build
.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.provides

examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.provides.build: examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o


examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o: examples/CMakeFiles/viewer_qt.dir/flags.make
examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o: ../examples/viewer/SurfaceRenderer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceRenderer.cpp

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceRenderer.cpp > CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.i

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceRenderer.cpp -o CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.s

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.requires:

.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.requires

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.provides: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/viewer_qt.dir/build.make examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.provides.build
.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.provides

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.provides.build: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o


examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o: examples/CMakeFiles/viewer_qt.dir/flags.make
examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o: ../examples/viewer/SurfaceVectorData.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceVectorData.cpp

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceVectorData.cpp > CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.i

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/viewer/SurfaceVectorData.cpp -o CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.s

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.requires:

.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.requires

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.provides: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/viewer_qt.dir/build.make examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.provides.build
.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.provides

examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.provides.build: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o


examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o: examples/CMakeFiles/viewer_qt.dir/flags.make
examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o: ../examples/viewer/SkullVolume.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/viewer/SkullVolume.cpp

examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/viewer/SkullVolume.cpp > CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.i

examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/viewer/SkullVolume.cpp -o CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.s

examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.requires:

.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.requires

examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.provides: examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/viewer_qt.dir/build.make examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.provides.build
.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.provides

examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.provides.build: examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o


examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o: examples/CMakeFiles/viewer_qt.dir/flags.make
examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o: ../examples/viewer/main-qt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/viewer/main-qt.cpp

examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/viewer/main-qt.cpp > CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.i

examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/viewer/main-qt.cpp -o CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.s

examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.requires:

.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.requires

examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.provides: examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/viewer_qt.dir/build.make examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.provides.build
.PHONY : examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.provides

examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.provides.build: examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o


# Object files for target viewer_qt
viewer_qt_OBJECTS = \
"CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o" \
"CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o" \
"CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o" \
"CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o" \
"CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o"

# External object files for target viewer_qt
viewer_qt_EXTERNAL_OBJECTS =

examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/build.make
examples/viewer_qt: ../lib/libugl-qt.so
examples/viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5OpenGL.so.5.9.1
examples/viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Widgets.so.5.9.1
examples/viewer_qt: /home/markus/Dokumente/Studium/SS-17/Projekt/ugl-master/libs/AntTweakBar/lib/libAntTweakBar.so
examples/viewer_qt: /usr/lib/x86_64-linux-gnu/libnetcdf.so
examples/viewer_qt: ../lib/libugl.so
examples/viewer_qt: /usr/lib/x86_64-linux-gnu/libGLEW.so
examples/viewer_qt: /usr/lib/x86_64-linux-gnu/libGL.so
examples/viewer_qt: /usr/lib/x86_64-linux-gnu/libboost_system.so
examples/viewer_qt: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
examples/viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Gui.so.5.9.1
examples/viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Core.so.5.9.1
examples/viewer_qt: examples/CMakeFiles/viewer_qt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable viewer_qt"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/viewer_qt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/viewer_qt.dir/build: examples/viewer_qt

.PHONY : examples/CMakeFiles/viewer_qt.dir/build

examples/CMakeFiles/viewer_qt.dir/requires: examples/CMakeFiles/viewer_qt.dir/viewer/Viewer.cpp.o.requires
examples/CMakeFiles/viewer_qt.dir/requires: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceRenderer.cpp.o.requires
examples/CMakeFiles/viewer_qt.dir/requires: examples/CMakeFiles/viewer_qt.dir/viewer/SurfaceVectorData.cpp.o.requires
examples/CMakeFiles/viewer_qt.dir/requires: examples/CMakeFiles/viewer_qt.dir/viewer/SkullVolume.cpp.o.requires
examples/CMakeFiles/viewer_qt.dir/requires: examples/CMakeFiles/viewer_qt.dir/viewer/main-qt.cpp.o.requires

.PHONY : examples/CMakeFiles/viewer_qt.dir/requires

examples/CMakeFiles/viewer_qt.dir/clean:
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/viewer_qt.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/viewer_qt.dir/clean

examples/CMakeFiles/viewer_qt.dir/depend:
	cd /home/markus/Downloads/ugl-prototype-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/markus/Downloads/ugl-prototype-master /home/markus/Downloads/ugl-prototype-master/examples /home/markus/Downloads/ugl-prototype-master/build /home/markus/Downloads/ugl-prototype-master/build/examples /home/markus/Downloads/ugl-prototype-master/build/examples/CMakeFiles/viewer_qt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/viewer_qt.dir/depend
