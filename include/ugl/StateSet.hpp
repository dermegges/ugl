/** @file StateSet.hpp



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

#ifndef __StateSet_hpp
#define __StateSet_hpp

#include <GL/glew.h>

#include "ModeSet.hpp"
#include "UniformSet.hpp"
#include "VariantProgram.hpp"

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <functional>

namespace ugl
{

/**
 * A hierarchical set of states used for rendering.
 *
 * RenderStages and \ref Drawable "Drawables" may have \ref StateSet "StateSets"
 * and the hierarchy of the \ref StateSet "StateSets" is determined by the call
 * sequence.
 *
 * If a state is not found in the current StateSet, its parents are
 * recursively visited to look for the state.
 */
class StateSet
{
private:
    boost::optional<UniformSet>    m_uniforms;
    boost::optional<ModeSet>       m_modes;
    boost::optional<glm::uvec4>    m_viewport;

    const StateSet*                m_parent;

    template <typename T>
    const T* getMember(
       std::function<const boost::optional<T>& (const StateSet*)> member) const;

    template <typename T>
    T& getOrCreateMember(boost::optional<T>& member);

public:
    StateSet();
    StateSet(const StateSet& parent);

    ShaderProgram& apply(VariantProgram& program) const;

    // methods for reading
    const UniformSet*    getUniforms()        const;
    const ModeSet*       getModes()           const;
    const glm::uvec4*    getViewport()        const;

    const StateSet* getParent() const;

    // methods for writing
    UniformSet&          getOrCreateUniforms();
    ModeSet&             getOrCreateModes();
    glm::uvec4&          getOrCreateViewport();

    void setParent(const StateSet& parent);
};

// ------------------------------------------------------------------------

template <typename T>
inline T& StateSet::getOrCreateMember(boost::optional<T>& member)
{
    if (!member)
        member = T();

    return *member;
}

// ------------------------------------------------------------------------

inline const StateSet* StateSet::getParent() const
{
    return m_parent;
}

// ------------------------------------------------------------------------

inline UniformSet& StateSet::getOrCreateUniforms()
{
    return getOrCreateMember(m_uniforms);
}

// ------------------------------------------------------------------------

inline ModeSet& StateSet::getOrCreateModes()
{
    return getOrCreateMember(m_modes);
}

// ------------------------------------------------------------------------

inline glm::uvec4& StateSet::getOrCreateViewport()
{
    return getOrCreateMember(m_viewport);
}

// ------------------------------------------------------------------------

inline void StateSet::setParent(const StateSet& parent)
{
    this->m_parent = &parent;

    this->getOrCreateUniforms().setParent(parent.getUniforms());
    this->getOrCreateModes().setParent(parent.getModes());
}

} // namespace ugl
#endif // __StateSet_hpp
