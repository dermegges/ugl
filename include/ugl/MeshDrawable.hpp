/** @file MeshDrawable.hpp



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

#ifndef _MESHDRAWABLE_HPP_
#define _MESHDRAWABLE_HPP_

#include <GL/glew.h>

#include "BoundingBox.hpp"
#include "Drawable.hpp"
#include "MeshData.hpp"
#include "StateSet.hpp"
#include "VariantProgram.hpp"

namespace ugl
{

/**
 * Basic Drawable which renders meshes provided by MeshData.
 */
class MeshDrawable : public Drawable
{
public:
    explicit MeshDrawable( MeshData& data );
    virtual ~MeshDrawable();
    virtual void draw( const StateSet& state );
    virtual BoundingBox getBoundingBox() const;

    void getTweakableParameters(bool** drawSurface, bool** drawEdges, float** alpha);

protected:
    /**
     * Set up shader program.
     */
    void prepareShaderProgram();

    void prepareVertexArrays( MeshData& data );

    VariantProgram m_program;
    StateSet       m_stateSet;

    GLuint         m_vertexArray;
    GLuint         m_vertexPositionBuffer;
    GLuint         m_vertexNormalBuffer;
    GLuint         m_triangleBuffer;
    GLsizei        m_triangleCount;
    GLuint         m_edgeBuffer;
    GLsizei        m_edgeCount;

    const BoundingBox m_boundingBox;

    bool           m_drawSurface;
    bool           m_drawEdges;
    float          m_alpha;
};


// -------------------------------------------------------------------------

inline MeshDrawable::~MeshDrawable()
{

}

// -------------------------------------------------------------------------

inline BoundingBox MeshDrawable::getBoundingBox() const
{
    return m_boundingBox;
}

} // namespace ugl
#endif // _MESHDRAWABLE_HPP_
