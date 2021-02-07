/** @file CurvesDrawable.hpp



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

#ifndef _CURVESDRAWABLE_HPP_
#define _CURVESDRAWABLE_HPP_

#include <vector>

#include <GL/glew.h>

#include "BoundingBox.hpp"
#include "CurvesData.hpp"
#include "Drawable.hpp"
#include "StateSet.hpp"
#include "Values.hpp"
#include "VariantProgram.hpp"

namespace ugl
{

/**
 * Basic Drawable which renders curves provided by CurvesData with optional
 * Values.
 *
 * There are three modes for the coloring of the curves:
 *
 *   * Without `Show Scalars` nor `Color Curves`, the curves are just white.
 *   * With `Color Curves` enabled, the color of each curve depends on the
 *     position of its seed (the start point) - the x coordinate corresponds to
 *     the red component, y to green and z to blue. Each individual curve has an
 *     constant color over its whole length.
 *   * If scalar values were added and `Show Scalars` is enabled, the scalar
 *     value for each point of the curve is shown, color coded between
 *     `Minimum Color` (default red) for small values and `Maximum Color`
 *     (default green) for big values. These modes and colors can be changed in
 *     the `Curves` TweakBar.
 */
class CurvesDrawable : public Drawable
{
public:
    explicit CurvesDrawable( CurvesData& data );
    virtual ~CurvesDrawable();
    virtual void draw( const StateSet& state );
    virtual BoundingBox getBoundingBox() const;
    void addValues(Values* values);    

    void getTweakableParameters(bool** colorCurves, float** alpha);

protected:
    /**
     * Set up shader program.
     */
    void prepareShaderProgram();

    void prepareVertexArrays( CurvesData& data );

    VariantProgram          m_program;
    StateSet                m_stateSet;

    GLuint                  m_vertexArray;
    GLuint                  m_vertexPositionBuffer;
    GLuint                  m_vertexCurveIdBuffer;
    GLuint                  m_curveColorBuffer;
    GLuint                  m_curveColorTex;
    std::vector<GLint>      m_startIndices;
    std::vector<GLsizei>    m_indexCounts;

    Values*                 m_values;

    const BoundingBox       m_boundingBox;

    bool                    m_colorCurves;
    float                   m_alpha;
};

// -------------------------------------------------------------------------

inline CurvesDrawable::~CurvesDrawable()
{
    delete m_values;
}

// -------------------------------------------------------------------------

inline BoundingBox CurvesDrawable::getBoundingBox() const
{
    return m_boundingBox;
}

} // namespace ugl
#endif // _CURVESDRAWABLE_HPP_
