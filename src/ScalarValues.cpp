/** @file ScalarValues.cpp



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

#include "ugl/ScalarValues.hpp"
#include "ugl/GLHelper.hpp"
#include "ugl/ModeSet.hpp"

namespace ugl
{

// -------------------------------------------------------------------------

ScalarValues::ScalarValues(ScalarData& data) :
    m_vertexScalarBuffer(GLHelper::prepareStaticBuffer(data.getAllScalars())),
    m_min(data.getMin()),
    m_max(data.getMax()),
    m_showScalars(false),
    m_minColor(1.0f, 0.0f, 0.0f),
    m_maxColor(0.0f, 1.0f, 0.0f)
{
}

// -------------------------------------------------------------------------

void ScalarValues::initialize(StateSet& stateSet, VariantProgram& program)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexScalarBuffer);

    const GLuint vertexScalarLocation = program.getUnusedAttributeLocation();
    program.addAttributeLocation("vertexScalar", vertexScalarLocation);

    glVertexAttribPointer(vertexScalarLocation, 1, GL_FLOAT,
                          GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexScalarLocation);

    glBindBuffer(GL_ARRAY_BUFFER, 0u);

    UniformSet& uniforms = stateSet.getOrCreateUniforms();
    uniforms.set("scalarsMin", m_min);
    uniforms.set("scalarsMax", m_max);
}

// -------------------------------------------------------------------------


void ScalarValues::getTweakableParameters(bool** showScalars, glm::vec3** minColor, glm::vec3** maxColor)
{
    *showScalars = &this->m_showScalars;
    *minColor = &this->m_minColor;
    *maxColor = &this->m_maxColor;
}


// -------------------------------------------------------------------------

void ScalarValues::extendDraw(StateSet& stateSet)
{
    ModeSet&    modes    = stateSet.getOrCreateModes();
    UniformSet& uniforms = stateSet.getOrCreateUniforms();

    modes.setOrClear("SHOW_SCALARS", m_showScalars);

    uniforms.set("minColor", m_minColor);
    uniforms.set("maxColor", m_maxColor);
}

} // namespace ugl
