#include "RedTriangle.hpp"

/**
 * @brief Constructor.
 */
RedTriangle::RedTriangle()
{
    // Init geometry
    this->vertices[0] = glm::vec3(-0.6f, 0.0f, 0.0f);
    this->vertices[1] = glm::vec3(0.6f, 0.0f, 0.0f);
    this->vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), this->vertices, GL_STATIC_DRAW);
    glVertexAttribPointer( 0u, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray( 0u );
    glBindBuffer( GL_ARRAY_BUFFER, 0u );
    glBindVertexArray(0u);

    // Init fullscreen quad
    glGenVertexArrays(1, &this->fullScreenQuadVAO);

    // Init shaders
    this->triangleProgram.addImportPath("shader");
    this->triangleProgram.addShaderFromSourceFile(ugl::COMBINED, "redTriangle.glsl");
    this->triangleProgram.addAttributeLocation("vertexPosition", 0u);

    this->displayProgram.addImportPath("shader");
    this->displayProgram.addShaderFromSourceFile(ugl::COMBINED, "textureDisplay.glsl");
}


/**
 * @brief Drawing.
 * @param state
 */
void RedTriangle::draw(const ugl::StateSet& state)
{
    ugl::StateSet myState;
    myState.setParent(state);

    // Save currently bound draw buffer
    GLint previousDrawBuffer;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawBuffer);

    // Resize offscreen framebuffer to match viewport
    const glm::uvec4& viewport = *(myState.getViewport());
    const unsigned int width  = viewport[2];
    const unsigned int height = viewport[3];

    // !!! NOTE !!!: If samples > 0, use GL_TEXTURE_2D_MULTISAMPLE as target for both. Also, you can change the formats of the color and depth buffer attachment here.
    this->framebuffer.resize(width, height, 0, GL_TEXTURE_RECTANGLE, GL_RGBA8, GL_TEXTURE_RECTANGLE, GL_DEPTH_COMPONENT32);
    this->framebuffer.checkCompleteness();

    // Bind offscreen buffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.getFramebuffer());

    // Clear it
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render triangle
    myState.apply(this->triangleProgram);
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);


    // Restore previous draw buffer
    glBindFramebuffer(GL_FRAMEBUFFER, previousDrawBuffer);

    // Set uniforms
    myState.getOrCreateUniforms().setTexture("colorTexture", GL_TEXTURE_RECTANGLE, this->framebuffer.getColorTexture());
    myState.getOrCreateUniforms().setTexture("depthTexture", GL_TEXTURE_RECTANGLE, this->framebuffer.getDepthTexture());

    // Render fullscreen quad
    myState.apply(this->displayProgram);

    glBindVertexArray(this->fullScreenQuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


/**
 * @brief Gets the bounding box.
 * @return
 */
ugl::BoundingBox RedTriangle::getBoundingBox() const
{
    ugl::BoundingBox box;
    for (int i = 0; i < 3; ++i)
        box.add(this->vertices[i]);
    return box;
}



