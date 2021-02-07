/** @file BoundingBox.cpp



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

#include "ugl/BoundingBox.hpp"

#include <limits>

namespace ugl
{


/**
 * @brief Default constructor.
 */
BoundingBox::BoundingBox() : min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
    max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max())
{
}


/**
 * @brief Explicit constructor.
 * @param min
 * @param max
 */
BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max) : min(min), max(max)
{
}


const BoundingBox& BoundingBox::add(const BoundingBox& other)
{
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);

    return *this;
}

const BoundingBox& BoundingBox::add(const glm::vec3& v)
{
    this->min = glm::min(this->min, v);
    this->max = glm::max(this->max, v);

    return *this;
}

float BoundingBox::diagonal() const
{
    return glm::distance(this->min, this->max);
}


// -------------------------------------------------------------------------

void BoundingBox::computeFromPositions(const std::vector<glm::vec3> positions)
{
    if (positions.size() > 0u)
    {
        min = max = positions.front();

        for (unsigned i = 1u; i < positions.size(); ++i)
        {
            min = glm::min(min, positions[i]);
            max = glm::max(max, positions[i]);
        }
    }
    else
    {
        min = max = glm::vec3(0.0f);
    }
}

} // namespace ugl
