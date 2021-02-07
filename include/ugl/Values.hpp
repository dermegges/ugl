/** @file Values.hpp



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

#ifndef _VALUES_HPP_
#define _VALUES_HPP_

#include "StateSet.hpp"
#include "VariantProgram.hpp"

namespace ugl
{

/**
 * Abstract base class for adding additional values to a Drawable and rendering
 * them.
 */
class Values
{
public:
    virtual ~Values();
    virtual void initialize(StateSet& stateSet, VariantProgram& program) = 0;
    virtual void extendDraw(StateSet& stateSet) = 0;
};

// -------------------------------------------------------------------------

inline Values::~Values()
{
}

} // namespace ugl
#endif // _VALUES_HPP_
