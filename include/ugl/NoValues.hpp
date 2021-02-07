/** @file NoValues.hpp



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

#ifndef _NOVALUES_HPP_
#define _NOVALUES_HPP_

#include "StateSet.hpp"
#include "Values.hpp"
#include "VariantProgram.hpp"

namespace ugl
{

/**
 * The most trivial subclass of Values which adds no values (all methods are
 * no-ops).
 *
 * Used by Drawables where Values should be optional.
 */
class NoValues : public Values
{
    virtual void initialize(StateSet& stateSet, VariantProgram& program);
    virtual void extendDraw(StateSet& stateSet);
};

inline void NoValues::initialize(StateSet& /*stateSet*/,
                                 VariantProgram& /*program*/)
{
}

// -------------------------------------------------------------------------

inline void NoValues::extendDraw(StateSet& /*stateSet*/)
{
}

} // namespace ugl
#endif // _NOVALUES_HPP_
