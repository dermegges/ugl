/** @file StateSet.cpp



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

#include "ugl/StateSet.hpp"
#include "ugl/ErrorCheck.hpp"

namespace ugl
{

StateSet::StateSet() :
    m_parent(nullptr)
{
}

StateSet::StateSet(const StateSet& parent)
{
    this->setParent(parent);
}


// ------------------------------------------------------------------------

template <typename T>
const T* StateSet::getMember(
        std::function<const boost::optional<T>& (const StateSet*)> member) const
{
    const StateSet* stateSet = this;

    do
    {
        if (member(stateSet))
            return member(stateSet).get_ptr();

        stateSet = stateSet->m_parent;
    } while (stateSet);

    return nullptr;
}

// ------------------------------------------------------------------------

ShaderProgram& StateSet::apply(VariantProgram& program) const
{
    ShaderProgram* compiled_program;
    const ModeSet* modes = getModes();
    if (modes)
        compiled_program = &(modes->apply(program));
    else
        compiled_program = &(program.bind());

    const UniformSet* uniforms = getUniforms();
    if (uniforms)
        uniforms->apply(compiled_program->programId());

    checkGLError();

    return *compiled_program;
}

// ------------------------------------------------------------------------

const UniformSet* StateSet::getUniforms() const
{
    return getMember<UniformSet>(
            [](const StateSet* stateSet) -> const boost::optional<UniformSet>&
            {return stateSet->m_uniforms;});
}

// ------------------------------------------------------------------------

const ModeSet* StateSet::getModes() const
{
    return getMember<ModeSet>(
            [](const StateSet* stateSet) -> const boost::optional<ModeSet>&
            {return stateSet->m_modes;});
}

// ------------------------------------------------------------------------

const glm::uvec4* StateSet::getViewport() const
{
    return getMember<glm::uvec4>(
            [](const StateSet* stateSet) -> const boost::optional<glm::uvec4>&
            {return stateSet->m_viewport;});
}

} // namespace ugl
