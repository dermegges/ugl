/** @file ScalarData.hpp



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

#ifndef _SCALARDATA_HPP_
#define _SCALARDATA_HPP_

#include <GL/glew.h>

#include <vector>

namespace ugl
{

/**
 * Data class for a list of scalar values.
 *
 * Usually used together in combination with another Data class to store
 * additionally values at the vertices.
 */
class ScalarData
{
public:
    ScalarData();

    GLsizei                     getScalarCount() const;
    GLfloat                     getScalar(GLint scalarId) const;
    const std::vector<GLfloat>& getAllScalars() const;

    void addScalar(GLfloat scalar);
    void setAllScalars(const std::vector<GLfloat>& scalars);

    GLfloat                     getMin();
    GLfloat                     getMax();
    void                        recomputeMinMax();

private:
    std::vector<GLfloat>    m_scalars;

    bool                    m_minMaxValid;
    GLfloat                 m_min;
    GLfloat                 m_max;
};

// -------------------------------------------------------------------------

inline ScalarData::ScalarData() :
    m_minMaxValid(false)
{
}

// -------------------------------------------------------------------------

inline GLsizei ScalarData::getScalarCount() const
{
    return m_scalars.size();
}

// -------------------------------------------------------------------------

inline GLfloat ScalarData::getScalar(GLint scalarId) const
{
    return m_scalars[scalarId];
}

// -------------------------------------------------------------------------

inline const std::vector<GLfloat>& ScalarData::getAllScalars() const
{
    return m_scalars;
}

// -------------------------------------------------------------------------

inline void ScalarData::addScalar(GLfloat scalar)
{
    m_scalars.push_back(scalar);
}

// -------------------------------------------------------------------------

inline void ScalarData::setAllScalars(const std::vector<GLfloat>& scalars)
{
    m_scalars = scalars;
}

// -------------------------------------------------------------------------

inline GLfloat ScalarData::getMin()
{
    if (!m_minMaxValid)
        recomputeMinMax();

    return m_min;
}

// -------------------------------------------------------------------------

inline GLfloat ScalarData::getMax()
{
    if (!m_minMaxValid)
        recomputeMinMax();

    return m_max;
}

} // namespace ugl
#endif // _SCALARDATA_HPP_
