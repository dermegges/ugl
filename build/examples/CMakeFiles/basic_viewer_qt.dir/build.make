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
include examples/CMakeFiles/basic_viewer_qt.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/basic_viewer_qt.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/basic_viewer_qt.dir/flags.make

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o: examples/CMakeFiles/basic_viewer_qt.dir/flags.make
examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o: ../examples/basic_viewer/BasicViewer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/BasicViewer.cpp

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/BasicViewer.cpp > CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.i

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/BasicViewer.cpp -o CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.s

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.requires:

.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.requires

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.provides: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/basic_viewer_qt.dir/build.make examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.provides.build
.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.provides

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.provides.build: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o


examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o: examples/CMakeFiles/basic_viewer_qt.dir/flags.make
examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o: ../examples/basic_viewer/main-qt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o -c /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/main-qt.cpp

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/main-qt.cpp > CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.i

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/examples/basic_viewer/main-qt.cpp -o CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.s

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.requires:

.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.requires

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.provides: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/basic_viewer_qt.dir/build.make examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.provides.build
.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.provides

examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.provides.build: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o


# Object files for target basic_viewer_qt
basic_viewer_qt_OBJECTS = \
"CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o" \
"CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o"

# External object files for target basic_viewer_qt
basic_viewer_qt_EXTERNAL_OBJECTS =

examples/basic_viewer_qt: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o
examples/basic_viewer_qt: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o
examples/basic_viewer_qt: examples/CMakeFiles/basic_viewer_qt.dir/build.make
examples/basic_viewer_qt: ../lib/libugl-qt.so
examples/basic_viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5OpenGL.so.5.9.1
examples/basic_viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Widgets.so.5.9.1
examples/basic_viewer_qt: /home/markus/Dokumente/Studium/SS-17/Projekt/ugl-master/libs/AntTweakBar/lib/libAntTweakBar.so
examples/basic_viewer_qt: ../lib/libugl.so
examples/basic_viewer_qt: /usr/lib/x86_64-linux-gnu/libGLEW.so
examples/basic_viewer_qt: /usr/lib/x86_64-linux-gnu/libGL.so
examples/basic_viewer_qt: /usr/lib/x86_64-linux-gnu/libboost_system.so
examples/basic_viewer_qt: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
examples/basic_viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Gui.so.5.9.1
examples/basic_viewer_qt: /opt/Qt/5.9.1/gcc_64/lib/libQt5Core.so.5.9.1
examples/basic_viewer_qt: examples/CMakeFiles/basic_viewer_qt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable basic_viewer_qt"
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/basic_viewer_qt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/basic_viewer_qt.dir/build: examples/basic_viewer_qt

.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/build

examples/CMakeFiles/basic_viewer_qt.dir/requires: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/BasicViewer.cpp.o.requires
examples/CMakeFiles/basic_viewer_qt.dir/requires: examples/CMakeFiles/basic_viewer_qt.dir/basic_viewer/main-qt.cpp.o.requires

.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/requires

examples/CMakeFiles/basic_viewer_qt.dir/clean:
	cd /home/markus/Downloads/ugl-prototype-master/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/basic_viewer_qt.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/clean

examples/CMakeFiles/basic_viewer_qt.dir/depend:
	cd /home/markus/Downloads/ugl-prototype-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/markus/Downloads/ugl-prototype-master /home/markus/Downloads/ugl-prototype-master/examples /home/markus/Downloads/ugl-prototype-master/build /home/markus/Downloads/ugl-prototype-master/build/examples /home/markus/Downloads/ugl-prototype-master/build/examples/CMakeFiles/basic_viewer_qt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/basic_viewer_qt.dir/depend

