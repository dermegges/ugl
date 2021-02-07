/** @file Volume.hpp



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

#ifndef __Volume_hpp
#define __Volume_hpp

#include <GL/glew.h>

#include "BoundingBox.hpp"
#include "MeshData.hpp"
#include "MeshDrawable.hpp"

namespace ugl
{

/**
 * Basic volume for volume rendering, similar to Drawable for mesh-based geometry.
 */
class Volume
{
public:
    Volume();
    virtual ~Volume();

    void setVisible(bool visible);
    virtual bool isVisible() const;

    void setRenderBoundingBoxDepthCube(bool renderDepthCube);
    virtual bool getRenderBoundingBoxDepthCube() const;

    MeshDrawable& getBoundingBoxDrawable();

public:
    virtual BoundingBox getBoundingBox() const = 0;

    virtual GLuint getIntensityTexture() const = 0;
    virtual glm::ivec3 getIntensityResolution() const = 0;

    virtual bool hasMaskTexture() const;
    virtual GLuint getMaskTexture() const;
    virtual glm::ivec3 getMaskResolution() const;

    virtual int getNumTransferTextures() const = 0;
    virtual GLuint getTransferArrayTexture() const = 0;
    virtual glm::vec4 getAlpha() const = 0;
    virtual int getTransferResolution() const = 0;    

    virtual float getSampleStepSize() const = 0;

protected:
    void initBoundingBoxDrawable();

private:
    bool visible;
    bool renderBoundingBoxDepthCube;

    MeshData* cubeData;
    MeshDrawable* cubeDrawable;
};

}
#endif
