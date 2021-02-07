µGL 
=============

A program and set of utilities which help writing visualization prototypes using OpenGL.

Supports order-independent transparency (OIT) using Depth Peeling or a Dynamic Fragment Buffer.


Compiling
---------

The program uses [CMake](http://www.cmake.org/) as its build system. It is cross-platform - it generates the files which are needed to build the program on your platform (e.g. makefiles). For this, go to the directory where the program should be build and run `cmake <path-to-source-directory>`. After that, the program can be compiled, e.g. by `make`.

Example: 
```
cd <ugl directory>

# Create a build directory to keep the sources clean:
mkdir build
cd build

# Call CMake and edit options (e.g. enable Qt-based examples):
ccmake ..

# Finally, you can build ugl:
make
```


Needed libraries:

  * [GLM](http://glm.g-truc.net/)
  * [Boost](http://www.boost.org/)
  * [GLEW (OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)
  * [AntTweakBar](http://anttweakbar.sourceforge.net/)
  * [NetCDF](http://www.unidata.ucar.edu/software/netcdf/)

For the GUI parts, you need *Qt*, *wxWidgets* or *SDL*

  * [Qt 5](http://qt-project.org/) (Core, GUI, OpenGL and Widgets)
  * [wxWidgets](http://wxwidgets.org/) (Base, Core and GL)
  * [SDL2](https://www.libsdl.org/)
  

Running
-------

Ugl tries to find its shaders using the UGL_DIR system environment variable which should point to the *root* directory of ugl (NOT the "shader" directory).

If the UGL_DIR environment variable is not set, ugl tries to find its shader code at "./shader", i.e., examples must be run from the ugl root directory.

On systems where the decimal separator is not `.`, the AntTweakBar has problems with floating points numbers. This can be solved by setting the environment variable `LC_NUMERIC` to `C`.

Altogether, an exemplary invocation looks like this:

    cd <path-to-source>
    LC_NUMERIC=C <path-to-executable>/viewer
    
    
Hints for using µGL with your project
-------------------------------------
Make sure you include the same version of GLM in your project as was used to compile µGL; otherwise, you will likely experience some severe errors or segmentation faults. The easiest way to achieve this is by adding `${UGL_DIR}/libs` to your include path.
  
  
  
License
-------------------------------------
Copyright 2016 Computational Topology Group, University of Kaiserslautern

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Authors: Christoph Garth, Tim Biedert
