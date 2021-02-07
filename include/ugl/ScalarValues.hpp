/** @file ScalarValues.hpp



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

#ifndef _SCALARVALUES_HPP_
#define _SCALARVALUES_HPP_

#include "ScalarData.hpp"
#include "StateSet.hpp"
#include "Values.hpp"
#include "VariantProgram.hpp"

#include <GL/glew.h>

#include <glm/glm.hpp>

namespace ugl
{

/**
 * Adds additional scalar values provided by ScalarData to a Drawable and
 * renders them.
 */
class ScalarValues : public Values
{
public:
    explicit ScalarValues(ScalarData& data);
    virtual void initialize(StateSet& stateSet, VariantProgram& program);    
    virtual void extendDraw(StateSet& stateSet);

    void getTweakableParameters(bool** showScalars, glm::vec3** minColor, glm::vec3** maxColor);

private:
    GLuint      m_vertexScalarBuffer;
    GLfloat     m_min;
    GLfloat     m_max;

    bool        m_showScalars;
    glm::vec3   m_minColor;
    glm::vec3   m_maxColor;
};

} // namespace ugl
#endif // _SCALARVALUES_HPP_
