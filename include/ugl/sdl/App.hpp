/** @file App.hpp



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

    Author(s): C.Garth, T.Biedert
*/

#ifndef _APP_HPP_
#define _APP_HPP_

#include "ugl/CoreApplication.hpp"

#include <string>


namespace ugl
{
namespace sdl
{

/**
 * Uses SDL 2 to create a window and run the given ugl core application in a simple event loop.
 */
class App
{
public:
    int run(ugl::CoreApplication* application, int width, int height, std::string title, bool showFPS, bool vsync, int major = 4, int minor = 0);
};

}
}
#endif
