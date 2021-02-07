/** @file ViewController3D.hpp



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

#ifndef __viewcontroller3d_hpp
#define __viewcontroller3d_hpp

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ViewController.hpp"
#include "BoundingBox.hpp"

namespace ugl
{

// ---------------------------------------------------------------

//! A simple but reasonably complete 3D view controller.
//!
//! Usage:
//!
//!     ViewController3D ctrl;
//!     // ...
//!     ctrl.begin( ViewController3D::ROTATE, mx, my );
//!     // ...
//!     ctrl.move( mx, my );
//!     // ...
//!     ctrl.end();
//!
//!     glm::mat4 modelview = ctrl.transform();
//!
//! Note: mouse coordinates are expected in [-1,1]^2
class ViewController3D : public ViewController
{
public:

    ViewController3D();

    virtual ~ViewController3D();

    virtual void init(const BoundingBox &boundingBox);

    virtual void resize( int width, int height );

    //!handle mouse press event; start interaction
    virtual bool onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);

    //!handle mouse move event; update interaction, if active
    virtual bool onMouseMove(int x, int y);

    //!handle mouse release event; finish interaction, if active
    virtual bool onMouseRelease(bool left, bool middle, bool right);

    virtual bool isInteracting() const;

    //! obtain current projection matrix  (optional aspect ratio parameter allows to retrieve the projection matrix adjusted for different window proportions, e.g., rendering to a pixelbuffer)
    virtual glm::mat4 projection(float aspectRatio = -1.0f) const;

    //! obtain current viewing matrix
    virtual glm::mat4 transform() const;
    //! obtain current tangent space matrix
    //! (for transforming normals etc.)
    virtual glm::mat3 rotation() const;

    //! set the home position
    void setHome( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up );
    //! set the home position such that the box given my min and max appears
    //! centered and in the right size
    void setHomeFromBoundingBox( const glm::vec3& min, const glm::vec3& max );
    //! reset view to home position
    virtual void reset();

    virtual void getView( glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up);
    virtual void setView( glm::vec3 eye, glm::vec3 center, glm::vec3 up);

protected:

    enum Mode
    {
        //! do nothing
        NONE,
        //! classic trackball rotation
        ROTATE,
        //! rotate around z-axis
        ROTATE_Z,
        //! pan rotation center parallel to image plane
        PAN,
        //! change distance from camera to center point
        ZOOM,
        //! move center point along look vector
        PUSH
    };

    //! begin movement in specific mode
    void begin( Mode mode, float mx, float my );
    //! move
    void move( float mx, float my );
    //! end movement
    void end();

    //! return the current interaction mode
    //Mode getMode() const;

    glm::vec3 project( const glm::vec2& s ) const;

    struct position
    {
        glm::vec3  center;
        glm::quat  rotation;
        float      distance;
    };

    position    m_pcurr;
    position    m_phome;
    float       m_aspectRatio;
    glm::vec2   m_mlast;
    Mode        m_mode;

    glm::ivec2 m_renderSize;
};

} // namespace ugl
#endif // __viewcontroller3d_hpp
