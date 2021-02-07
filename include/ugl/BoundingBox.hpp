/** @file BoundingBox.hpp



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

#ifndef _BOUNDINGBOX_HPP_
#define _BOUNDINGBOX_HPP_

#include <glm/glm.hpp>

#include <vector>

namespace ugl
{

/**
 * A cuboidal bounding box with the possibility to add multiple BoundingBoxes or
 * compute one from a vector of positions.
 */
class BoundingBox
{
public:
    BoundingBox();
    BoundingBox(const glm::vec3& min, const glm::vec3& max);

    const glm::vec3& getMin() const;
    const glm::vec3& getMax() const;
    glm::vec3 normalizePosition(const glm::vec3& position) const;

    float diagonal() const;

    const BoundingBox &add(const BoundingBox& other);
    const BoundingBox &add(const glm::vec3& v);
    void computeFromPositions(const std::vector<glm::vec3> positions);
    void clear(glm::vec3 v = glm::vec3(0,0,0));

private:
    glm::vec3 min;
    glm::vec3 max;
};

// -------------------------------------------------------------------------

inline const glm::vec3& BoundingBox::getMin() const
{
    return min;
}

// -------------------------------------------------------------------------

inline const glm::vec3& BoundingBox::getMax() const
{
    return max;
}

// -------------------------------------------------------------------------

inline glm::vec3 BoundingBox::normalizePosition(const glm::vec3& position) const
{
    return (position - min) / (max - min);
}

// -------------------------------------------------------------------------

inline void BoundingBox::clear(glm::vec3 v) //Default: (0,0,0)
{
    this->min = v;
    this->max = v;
}

} // namespace ugl
#endif // _BOUNDINGBOX_HPP_
