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
include src/CMakeFiles/ugl-qt.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ugl-qt.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ugl-qt.dir/flags.make

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o: src/CMakeFiles/ugl-qt.dir/flags.make
src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o: ../src/qt/Widget.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o -c /home/markus/Downloads/ugl-prototype-master/src/qt/Widget.cpp

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ugl-qt.dir/qt/Widget.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/src/qt/Widget.cpp > CMakeFiles/ugl-qt.dir/qt/Widget.cpp.i

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ugl-qt.dir/qt/Widget.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/src/qt/Widget.cpp -o CMakeFiles/ugl-qt.dir/qt/Widget.cpp.s

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.requires:

.PHONY : src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.requires

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.provides: src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ugl-qt.dir/build.make src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.provides.build
.PHONY : src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.provides

src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.provides.build: src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o


src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o: src/CMakeFiles/ugl-qt.dir/flags.make
src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o: ../src/qt/OpenGLWidget.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o -c /home/markus/Downloads/ugl-prototype-master/src/qt/OpenGLWidget.cpp

src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/src/qt/OpenGLWidget.cpp > CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.i

src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/src/qt/OpenGLWidget.cpp -o CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.s

src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.requires:

.PHONY : src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.requires

src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.provides: src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ugl-qt.dir/build.make src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.provides.build
.PHONY : src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.provides

src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.provides.build: src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o


src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o: src/CMakeFiles/ugl-qt.dir/flags.make
src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o: src/ugl-qt_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o -c /home/markus/Downloads/ugl-prototype-master/build/src/ugl-qt_automoc.cpp

src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.i"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/markus/Downloads/ugl-prototype-master/build/src/ugl-qt_automoc.cpp > CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.i

src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.s"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/markus/Downloads/ugl-prototype-master/build/src/ugl-qt_automoc.cpp -o CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.s

src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.requires:

.PHONY : src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.requires

src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.provides: src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/ugl-qt.dir/build.make src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.provides.build
.PHONY : src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.provides

src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.provides.build: src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o


# Object files for target ugl-qt
ugl__qt_OBJECTS = \
"CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o" \
"CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o" \
"CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o"

# External object files for target ugl-qt
ugl__qt_EXTERNAL_OBJECTS =

../lib/libugl-qt.so: src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o
../lib/libugl-qt.so: src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o
../lib/libugl-qt.so: src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o
../lib/libugl-qt.so: src/CMakeFiles/ugl-qt.dir/build.make
../lib/libugl-qt.so: ../lib/libugl.so
../lib/libugl-qt.so: /opt/Qt/5.9.1/gcc_64/lib/libQt5OpenGL.so.5.9.1
../lib/libugl-qt.so: /opt/Qt/5.9.1/gcc_64/lib/libQt5Widgets.so.5.9.1
../lib/libugl-qt.so: /usr/lib/x86_64-linux-gnu/libGLEW.so
../lib/libugl-qt.so: /usr/lib/x86_64-linux-gnu/libGL.so
../lib/libugl-qt.so: /usr/lib/x86_64-linux-gnu/libboost_system.so
../lib/libugl-qt.so: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
../lib/libugl-qt.so: /opt/Qt/5.9.1/gcc_64/lib/libQt5Gui.so.5.9.1
../lib/libugl-qt.so: /opt/Qt/5.9.1/gcc_64/lib/libQt5Core.so.5.9.1
../lib/libugl-qt.so: src/CMakeFiles/ugl-qt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/markus/Downloads/ugl-prototype-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../../lib/libugl-qt.so"
	cd /home/markus/Downloads/ugl-prototype-master/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ugl-qt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ugl-qt.dir/build: ../lib/libugl-qt.so

.PHONY : src/CMakeFiles/ugl-qt.dir/build

src/CMakeFiles/ugl-qt.dir/requires: src/CMakeFiles/ugl-qt.dir/qt/Widget.cpp.o.requires
src/CMakeFiles/ugl-qt.dir/requires: src/CMakeFiles/ugl-qt.dir/qt/OpenGLWidget.cpp.o.requires
src/CMakeFiles/ugl-qt.dir/requires: src/CMakeFiles/ugl-qt.dir/ugl-qt_automoc.cpp.o.requires

.PHONY : src/CMakeFiles/ugl-qt.dir/requires

src/CMakeFiles/ugl-qt.dir/clean:
	cd /home/markus/Downloads/ugl-prototype-master/build/src && $(CMAKE_COMMAND) -P CMakeFiles/ugl-qt.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ugl-qt.dir/clean

src/CMakeFiles/ugl-qt.dir/depend:
	cd /home/markus/Downloads/ugl-prototype-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/markus/Downloads/ugl-prototype-master /home/markus/Downloads/ugl-prototype-master/src /home/markus/Downloads/ugl-prototype-master/build /home/markus/Downloads/ugl-prototype-master/build/src /home/markus/Downloads/ugl-prototype-master/build/src/CMakeFiles/ugl-qt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ugl-qt.dir/depend

