#ifndef SURFACERENDERER_HPP
#define SURFACERENDERER_HPP

#include <utility>
#include <vector>

#include <glm/glm.hpp>

#include <AntTweakBar.h>

#include <ugl/StateSet.hpp>
#include <ugl/Drawable.hpp>
#include <ugl/VariantProgram.hpp>

#include "SurfaceVectorData.hpp"

// ------------------------------------------------------------------------

/**
 * Drawable which renders a surface provided by SurfaceVectorData.
 *
 * Should be replaced by MeshDrawable.
 */
class SurfaceRenderer : public ugl::Drawable
{
public:

    SurfaceRenderer( const SurfaceVectorData& svd );

    virtual void draw( const ugl::StateSet& state );
    virtual ugl::BoundingBox getBoundingBox() const;

protected:

    static std::vector<glm::vec3>
    computeFaceNormals( const SurfaceVectorData& svd );

    static std::vector<glm::vec3>
    computeVertexNormals( const SurfaceVectorData& svd );

    static std::vector<glm::uvec2>
    computeEdges( const SurfaceVectorData& svd );

    static std::pair<glm::vec3, glm::vec3>
    computeBoundingBox( const SurfaceVectorData& svd );

    void prepareVertexArrays( const SurfaceVectorData& svd );
    void prepareShaderProgram();

    GLuint                  m_vertexArray;

    GLuint                  m_vertexBuffer;
    GLuint                  m_vertexNormalBuffer;
    GLuint                  m_vectorBuffer;

    GLuint                  m_faceNormalBuffer;
    GLuint                  m_faceNormalTex;

    GLuint                  m_edgeIndexBuffer;
    GLuint                  m_edgeIndexSize;

    GLuint                  m_triIndexBuffer;
    GLuint                  m_triIndexSize;

    ugl::BoundingBox        m_boundingBox;

    ugl::VariantProgram     m_program;

    bool                    m_drawEdges;
    bool                    m_drawSurface;
    bool                    m_showVectors;
    bool                    m_useVertexNormal;
    float                   m_alpha;

    ugl::StateSet           m_stateSet;
};

// ------------------------------------------------------------------------

inline ugl::BoundingBox SurfaceRenderer::getBoundingBox() const
{
    return m_boundingBox;
}

#endif // SURFACERENDERER_HPP
