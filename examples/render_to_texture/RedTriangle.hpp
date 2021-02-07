#ifndef REDTRIANGLE_H
#define REDTRIANGLE_H

#include <glm/glm.hpp>

#include <ugl/Drawable.hpp>
#include <ugl/VariantProgram.hpp>
#include <ugl/Framebuffer.hpp>


/**
 * @brief This drawable consists of two passes:  1) render triangle to texture   2) display texture
 */
class RedTriangle : public ugl::Drawable
{
public:
    RedTriangle();
    virtual ~RedTriangle() {}

public:
    void draw(const ugl::StateSet& state);
    ugl::BoundingBox getBoundingBox() const;

private:
    glm::vec3 vertices[3];
    GLuint vao;

    GLuint fullScreenQuadVAO;

    ugl::VariantProgram triangleProgram;
    ugl::VariantProgram displayProgram;

    ugl::Framebuffer framebuffer;
};

#endif


