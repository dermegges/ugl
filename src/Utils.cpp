/** @file Utils.cpp



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

#include "ugl/Utils.hpp"

#include <iostream>
#include <string>
#include <cstdlib>

namespace ugl
{
/**
 * @brief Tries to determine the ugl base directory from the UGL_DIR system environment variable.
 * @return "." if the variable is not set.
 */
std::string getBaseDir()
{
    const char* UGL_DIR = std::getenv("UGL_DIR");

    if (UGL_DIR != 0)
    {
        return std::string(UGL_DIR);
    }
    else
    {
//        std::cerr << "Warning: UGL_DIR environment variable is not set!" << std::endl;
        return ".";
    }
}
}
