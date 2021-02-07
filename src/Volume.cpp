/** @file Volume.cpp



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

#include "ugl/Volume.hpp"

namespace ugl
{

/**
 * @brief Constructor.
 */
Volume::Volume() : visible(true), renderBoundingBoxDepthCube(true), cubeData(0), cubeDrawable(0)
{
}


/**
 * @brief Destructor.
 */
Volume::~Volume()
{
    if (this->cubeDrawable != 0)
        delete this->cubeDrawable;

    if (this->cubeData != 0)
        delete this->cubeData;
}


/**
 * @brief Sets the volume visibility.
 * @param visible
 */
void Volume::setVisible(bool visible)
{
    this->visible = visible;
}


/**
 * @brief Returns volume visibility.
 * @return
 */
bool Volume::isVisible() const
{
    return this->visible;
}


/**
 * @brief Enables (depth-only) rendering of the bounding box cube.
 * @param renderDepthCube
 */
void Volume::setRenderBoundingBoxDepthCube(bool renderDepthCube)
{
    this->renderBoundingBoxDepthCube = renderDepthCube;
}


/**
 * @brief Returns true if the bounding box cube is rendered using depth-only mode.
 * @return
 */
bool Volume::getRenderBoundingBoxDepthCube() const
{
    return this->renderBoundingBoxDepthCube;
}


/**
 * @brief Returns the drawable cube of the volume bounding box.
 * @return
 */
MeshDrawable &Volume::getBoundingBoxDrawable()
{
    if (this->cubeDrawable == 0)
        this->initBoundingBoxDrawable();

    return *this->cubeDrawable;
}



/**
 * @brief Initialize internal volume cube representation.
 */
void Volume::initBoundingBoxDrawable()
{
    // Clean previous
    if (this->cubeDrawable != 0)
        delete this->cubeDrawable;

    if (this->cubeData != 0)
        delete this->cubeData;

    // Create new
    this->cubeData = new MeshData();

    ugl::BoundingBox boundingBox = this->getBoundingBox();
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();

    this->cubeData->addPoint(glm::vec3(min.x, min.y, min.z));
    this->cubeData->addPoint(glm::vec3(max.x, min.y, min.z));
    this->cubeData->addPoint(glm::vec3(max.x, max.y, min.z));
    this->cubeData->addPoint(glm::vec3(min.x, max.y, min.z));
    this->cubeData->addPoint(glm::vec3(min.x, min.y, max.z));
    this->cubeData->addPoint(glm::vec3(max.x, min.y, max.z));
    this->cubeData->addPoint(glm::vec3(max.x, max.y, max.z));
    this->cubeData->addPoint(glm::vec3(min.x, max.y, max.z));

    this->cubeData->addTriangle(glm::uvec3(0, 1, 2));
    this->cubeData->addTriangle(glm::uvec3(0, 2, 3));

    this->cubeData->addTriangle(glm::uvec3(4, 6, 5));
    this->cubeData->addTriangle(glm::uvec3(4, 7, 6));

    this->cubeData->addTriangle(glm::uvec3(1, 5, 6));
    this->cubeData->addTriangle(glm::uvec3(1, 6, 2));

    this->cubeData->addTriangle(glm::uvec3(0, 3, 4));
    this->cubeData->addTriangle(glm::uvec3(4, 3, 7));

    this->cubeData->addTriangle(glm::uvec3(3, 2, 7));
    this->cubeData->addTriangle(glm::uvec3(2, 6, 7));

    this->cubeData->addTriangle(glm::uvec3(0, 4, 1));
    this->cubeData->addTriangle(glm::uvec3(1, 4, 5));

    this->cubeDrawable = new MeshDrawable(*this->cubeData);
}


/**
 * @brief Returns whether the volume has a mask, i.e., a texture describing where volume rendering should (not) be done.
 * @return
 */
bool Volume::hasMaskTexture() const
{
    return false;
}


/**
 * @brief Returns the masking texture.
 * @return
 */
GLuint Volume::getMaskTexture() const
{
    return 0;
}


/**
 * @brief Returns the resolution of the mask texture.
 * @return
 */
glm::ivec3 Volume::getMaskResolution() const
{
    return this->getIntensityResolution();
}


}
