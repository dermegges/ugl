/** @file ErrorCheck.hpp



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

#ifndef ERRORCHECK_HPP
#define ERRORCHECK_HPP

#include <string>

namespace ugl
{
void checkAndPrintGLError(std::string label, const char* file, int line);

#define checkGLError() { ugl::checkAndPrintGLError("",  __FILE__, __LINE__ ); }
#define checkGLErrorLabel(label) { ugl::checkAndPrintGLError(label,  __FILE__, __LINE__ ); }
}

#endif // ERRORCHECK_HPP
