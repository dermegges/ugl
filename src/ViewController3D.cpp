/** @file ViewController3D.cpp



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

#define _USE_MATH_DEFINES

#include "ugl/ViewController3D.hpp"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>
#include <cmath>


namespace ugl
{

// -------------------------------------------------------------------------

glm::quat rotate( const glm::quat& q, glm::vec3 src, glm::vec3 dst )
{
    src = glm::normalize( src );
    dst = glm::normalize( dst );

    float dotProdPlus1 = 1.0f + glm::dot( src, dst );

    glm::quat r;

    if( dotProdPlus1 < 1e-7f )
    {
        if( std::abs(src.x) < 0.6f )
        {
            const float norm = sqrt( 1.0f - src.x*src.x );
            r = glm::quat( 0.0f, src.z/norm, -src.y/norm, 0.0f ) * q;
        }
        else if( std::abs(src.y) < 0.6f )
        {
            const float norm = sqrt(1.0f - src.y*src.y);
            r = glm::quat( -src.z/norm, 0.0f, src.x/norm, 0.0f ) * q;
        }
        else
        {
            const float norm = sqrt( 1.0f - src.z*src.z);
            r = glm::quat( src.y/norm, -src.x/norm, 0.0f, 0.0f ) * q;
        }
    }
    else
    {
        const float     s = sqrt( 0.5f * dotProdPlus1 );
        const glm::vec3 a = 0.5f * glm::cross( src, dst ) / s;

        r = glm::quat( s, a );
    }

    return r * q;
}


// -------------------------------------------------------------------------

void ViewController3D::init(const BoundingBox &boundingBox)
{
    setHomeFromBoundingBox(boundingBox.getMin(), boundingBox.getMax());
}

// -------------------------------------------------------------------------

glm::vec3 ViewController3D::project( const glm::vec2& s ) const
{
    const float trsize = 0.8f;
    const float trssqr = trsize * trsize / 2.0f;

    float l2 = glm::dot( s, s );

    // plane points projected to ViewController3D
    return glm::vec3( s, l2 < trssqr ? sqrt(2.0f*trssqr - l2) : trssqr/sqrt(l2) );
}

// -------------------------------------------------------------------------

ViewController3D::ViewController3D()
{
    glm::mat4 m = glm::lookAt( glm::vec3( 0.0f, 0.0f, -3.0f ),
                               glm::vec3( 0.0f, 0.0f, 0.0f  ),
                               glm::vec3( 0.0f, 1.0f, 0.0f  ) );

    m_mode = NONE;

    m_phome.center   = glm::vec3( 0.0f, 0.0f, 0.0f );
    m_phome.rotation = glm::quat( glm::inverseTranspose( glm::mat3( m ) ) );
    m_phome.distance = 3.0f;

    m_pcurr = m_phome;
}

// -------------------------------------------------------------------------

ViewController3D::~ViewController3D()
{
}

// -------------------------------------------------------------------------

void ViewController3D::begin( Mode mode, float mx, float my )
{
    m_mlast = glm::vec2( mx, my );
    m_mode  = mode;
}

// -------------------------------------------------------------------------

void ViewController3D::end()
{
    m_mode = NONE;
}

// -------------------------------------------------------------------------

//ViewController3D::Mode ViewController3D::getMode() const
//{
//    return m_mode;
//}

// -------------------------------------------------------------------------

void ViewController3D::move( float mx, float my )
{
    glm::vec2 mcurr = glm::vec2( mx, my );
    glm::vec2 delta = glm::vec2( mcurr - m_mlast );

    switch( m_mode )
    {
    case ROTATE:
        m_pcurr.rotation = rotate( m_pcurr.rotation, project( m_mlast ), project( mcurr ) );
        break;
    case ROTATE_Z:
        m_pcurr.rotation = rotate( m_pcurr.rotation, glm::vec3( m_mlast, 0.0f ), glm::vec3( mcurr, 0.0f ) );
        break;
    case ZOOM:
        m_pcurr.distance *= 1.0f + delta.y;
        break;
    case PUSH:
        m_pcurr.center += glm::rotate( glm::conjugate( m_pcurr.rotation ), glm::vec3( 0.0f, 0.0f, delta.y*m_pcurr.distance ) );
        break;
    case PAN:
        delta *= 0.3f * m_pcurr.distance;
        m_pcurr.center -= glm::rotate( glm::conjugate( m_pcurr.rotation ), glm::vec3( delta, 0.0f ) );
        break;
    default:
        break;
    }

    m_mlast = mcurr;
}

// -------------------------------------------------------------------------

glm::mat4 ViewController3D::transform() const
{
    glm::mat4 t;

    t = glm::translate( t, glm::vec3( 0.0f, 0.0f, -m_pcurr.distance ) );
    t *= glm::mat4_cast( m_pcurr.rotation );
    t = glm::translate( t, -m_pcurr.center );

    return t;
}

// -------------------------------------------------------------------------

glm::mat3 ViewController3D::rotation() const
{
    return glm::inverseTranspose( glm::mat3( transform() ) );
}

// -------------------------------------------------------------------------

void ViewController3D::setHome( const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up )
{
    glm::mat4 look = glm::lookAt( eye, center, up );

    m_phome.center   = center;
    m_phome.rotation = glm::quat( glm::inverseTranspose( glm::mat3( look ) ) );
    m_phome.distance = glm::length(eye - center);
}

// -------------------------------------------------------------------------

void ViewController3D::setHomeFromBoundingBox( const glm::vec3& min,
                                               const glm::vec3& max )
{
    glm::vec3 center = ( min + max ) / 2.0f;
    float radius = glm::length( center - min );

    setHome( center + glm::vec3( 0.0f, 0.0f, 3.0f * radius ),
             center,
             glm::vec3( 0.0f, 1.0f, 0.0f ) );
}

// -------------------------------------------------------------------------

void ViewController3D::reset()
{
    m_pcurr = m_phome;
}

// -------------------------------------------------------------------------

void ViewController3D::resize( int width, int height )
{
    m_aspectRatio = static_cast<float>( width ) / static_cast<float>( height );
    m_renderSize = glm::ivec2(width,height);
}

// -------------------------------------------------------------------------

void ViewController3D::getView( glm::vec3 &eye, glm::vec3 &center, glm::vec3 &up)
{
    glm::mat4 mat = transform();
    up = glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
    eye = glm::vec3(glm::vec4(0,0,0,1)*glm::inverseTranspose(mat));
    center = m_pcurr.center;
}

// -------------------------------------------------------------------------

void ViewController3D::setView( glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    setHome(eye, center, up);
    reset();
}

// -------------------------------------------------------------------------

bool ViewController3D::onMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt)
{
    Mode mode;

    if(left)
    {
        if(shift)
            mode = ROTATE_Z;
        else if(alt)
            mode = ZOOM;
        else if(control)
            mode = PAN;
        else
            mode = ROTATE;
    }
    else if(right)
    {
        if(shift)
            mode = PUSH;
        else
            mode = ZOOM;
    }
    else if(middle)
    {
        mode = PAN;
    }
    else
        return false;

    int d = std::max(m_renderSize.x, m_renderSize.y);

    float mx = 2.0f * x/(float)d - 1.0f;
    float my = 1.0f - 2.0f * y/(float)d;

    begin( mode, mx, my );

    return true;
}

// -------------------------------------------------------------------------

bool ViewController3D::onMouseMove(int x, int y)
{
    if( m_mode != NONE )
    {
        //int d = std::max(m_renderSize.x, m_renderSize.y);
        int d = std::max(m_renderSize.x, m_renderSize.y);

        float mx = 2.0f * x/(float)d - 1.0f;
        float my = 1.0f - 2.0f * y/(float)d;

        move( mx, my );
        return true;
    }
    else
    {
        return false;
    }
}


// -------------------------------------------------------------------------

bool ViewController3D::onMouseRelease(bool, bool, bool)
{
    if( m_mode != NONE )
    {
        end();
        return true;
    }
    else
    {
        return false;
    }
}

// -------------------------------------------------------------------------

bool ViewController3D::isInteracting() const
{
    return (m_mode != NONE);
}

// -------------------------------------------------------------------------

glm::mat4 ViewController3D::projection(float aspectRatio) const
{
    float aspect = (aspectRatio > 0.0 ? aspectRatio : this->m_aspectRatio);

    const float minimalAngle = 45.0f;
    const float tangent =
            tan( minimalAngle * static_cast<float>( M_PI ) / 360.0f );

    // If the width is less then the height, set the vertical angle such that
    // the horizontal angle is 45°.
    const float verticalAngle = aspect >= 1.0f ? minimalAngle :
        atan( tangent / aspect ) * 360.0f / static_cast<float>( M_PI );

    glm::mat4 pr = glm::perspective(
            glm::radians(verticalAngle), aspect,
            0.1f * m_pcurr.distance, 10.0f * m_pcurr.distance );

    return pr;
}

} // namespace ugl
