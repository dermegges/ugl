/** @file ModeSet.hpp



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

#ifndef _MODESET_HPP_
#define _MODESET_HPP_

#include <GL/glew.h>

#include "AbstractValueSet.hpp"
#include "VariantProgram.hpp"

#include <boost/variant.hpp>

#include <string>
#include <map>


namespace ugl
{

/**
 * A hierarchical set of preprocessor define modes for use with the
 * GLSLPreprocessor.
 */
class ModeSet : public AbstractValueSet< boost::variant<long, std::string> >
{
public:

    void set( const std::string& name,
              const boost::variant<long, std::string>& value );
    void setOrClear( const std::string& name, bool shouldSet );
    ShaderProgram& apply( VariantProgram& program ) const;
};

// -------------------------------------------------------------------------

inline void ModeSet::set( const std::string& name,
                          const boost::variant<long, std::string>& value )
{
    m_values[name] = value;
}

// -------------------------------------------------------------------------

inline void ModeSet::setOrClear( const std::string& name, bool shouldSet )
{
    if (shouldSet)
        set(name, 1);
    else
        clear(name);
}

// -------------------------------------------------------------------------

inline ShaderProgram& ModeSet::apply( VariantProgram& program ) const
{
    return program.bind(getMergedMap());
}

// -------------------------------------------------------------------------

} // namespace ugl
#endif
