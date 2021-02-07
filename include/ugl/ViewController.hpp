/** @file ViewController.hpp



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

#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "BoundingBox.hpp"

#include <glm/glm.hpp>


namespace ugl
{

/**
 * @brief Abstract view controller interface.
 */
class ViewController
{
public:
    ViewController() {}
    virtual ~ViewController() {}

    /**
     * @brief Set home from bounding box
     */
    virtual void init(const BoundingBox& boundingBox) = 0;
    virtual void reset() = 0;

    virtual void resize(int width, int height) = 0;

    virtual bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt) = 0;
    virtual bool onMouseMove(int x, int y) = 0;
    virtual bool onMouseRelease(bool left, bool middle, bool right) = 0;

    virtual bool isInteracting() const = 0;

    virtual glm::mat4 projection(float aspectRatio = -1.0f) const = 0;
    virtual glm::mat4 transform() const = 0;
    virtual glm::mat3 rotation() const = 0;

    virtual void getView( glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up) = 0;
    virtual void setView( glm::vec3 eye, glm::vec3 center, glm::vec3 up) = 0;
};

}

#endif // VIEWCONTROLLER_H
