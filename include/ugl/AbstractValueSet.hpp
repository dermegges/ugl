/** @file AbstractValueSet.hpp



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

#ifndef _ABSTRACTVALUESET_HPP_
#define _ABSTRACTVALUESET_HPP_

#include <map>
#include <stack>
#include <string>

namespace ugl
{

// -------------------------------------------------------------------------

/**
 * A hierarchical set of values of template-type T which can be accessed by a
 * name (as string).
 *
 * If the value is not found in the current ValueSet, its parents are
 * recursively visited to look for the value.
 *
 * Used as the base class for ModeSet and UniformSet.
 */
template <typename T>
class AbstractValueSet
{
public:
    AbstractValueSet();

    void clear( const std::string& name );
    void clear();

    bool exists( const std::string& name ) const;
    const T* get( const std::string& name ) const;

    void setParent(const AbstractValueSet* parent);

protected:

    typedef std::map< std::string, T > named_value_map;

    named_value_map m_values;

    const AbstractValueSet* m_parent;

    named_value_map getMergedMap() const;
};

// -------------------------------------------------------------------------

template <typename T>
AbstractValueSet<T>::AbstractValueSet() :
    m_parent(nullptr)
{
}

// -------------------------------------------------------------------------

template <typename T>
void AbstractValueSet<T>::clear( const std::string& name )
{
    typename named_value_map::iterator it = m_values.find( name );

    if( it != m_values.end() )
        m_values.erase( it );
}

// -------------------------------------------------------------------------

template <typename T>
void AbstractValueSet<T>::clear()
{
    m_values.clear();
}

// -------------------------------------------------------------------------

template <typename T>
bool AbstractValueSet<T>::exists( const std::string& name ) const
{
    return !!get( name );
}

// -------------------------------------------------------------------------

template <typename T>
const T* AbstractValueSet<T>::get( const std::string& name ) const
{
    const AbstractValueSet* valueSet = this;

    do
    {
        typename named_value_map::const_iterator it =
                valueSet->m_values.find(name);
        if (it != valueSet->m_values.end())
        {
            return &(it->second);
        }

        valueSet = valueSet->m_parent;
    } while (valueSet);

    return nullptr;
}

// -------------------------------------------------------------------------

template <typename T>
inline void AbstractValueSet<T>::setParent(const AbstractValueSet* parent)
{
    m_parent = parent;
}

// -------------------------------------------------------------------------

template <typename T>
typename AbstractValueSet<T>::named_value_map
        AbstractValueSet<T>::getMergedMap() const
{
    std::stack<const AbstractValueSet*> parents;
    const AbstractValueSet* valueSet = this;

    // Walk to top, push all ValueSets on stack.
    do
    {
        parents.push(valueSet);
        valueSet = valueSet->m_parent;
    } while (valueSet);

    // Merge all value maps from top to bottom
    named_value_map out(parents.top()->m_values);
    parents.pop();
    while (!parents.empty())
    {
        valueSet = parents.top();
        out.insert(valueSet->m_values.begin(), valueSet->m_values.end());
        parents.pop();
    }

    return out;
}

// -------------------------------------------------------------------------

} // namespace ugl
#endif
