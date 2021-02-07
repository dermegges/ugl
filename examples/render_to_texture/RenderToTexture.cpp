#include "RenderToTexture.hpp"

#include <iostream>
#include <algorithm>
#include <sstream>


/**
 * @brief Constructor.
 */
RenderToTexture::RenderToTexture() : transparentRenderStage(nullptr), drawable(nullptr)
{
}


/**
 * @brief Destructor.
 */
RenderToTexture::~RenderToTexture()
{
    if (this->transparentRenderStage != nullptr)
        delete this->transparentRenderStage;

    if (this->drawable != nullptr)
        delete this->drawable;
}


/**
 * @brief Initialization.
 */
void RenderToTexture::initGL()
{
    this->drawable = new RedTriangle();

    // Use bounding box to set reasonable home for the ViewController.
    ugl::BoundingBox box = this->drawable->getBoundingBox();
    ugl::ViewController &viewCtrl = *(getViewController());
    viewCtrl.init(box);
    viewCtrl.reset();

    transparentRenderStage = new ugl::TransparentRenderStage();
    transparentRenderStage->addDrawable(this->drawable);

}


/**
 * @brief Rendering.
 */
void RenderToTexture::drawGL()
{
    const ugl::ViewController &viewCtrl = *(getViewController());

    glm::ivec2 size = this->getRenderSize();
    stateSet.getOrCreateViewport() = glm::uvec4(0u, 0u, size.x, size.y);

    ugl::UniformSet& uniforms = stateSet.getOrCreateUniforms();

    uniforms.set("globals.projectionMatrix", viewCtrl.projection());
    uniforms.set("globals.modelviewMatrix",  viewCtrl.transform());
    uniforms.set("globals.normalMatrix",     viewCtrl.rotation());

    uniforms.set("globals.light[0].direction",  glm::normalize(glm::vec3(0.1f, 0.1f, -1.0f)));
    uniforms.set("globals.light[0].glossiness", 16.0f);

    uniforms.set("globals.defaultColor", glm::vec3(1.0f, 0.8f, 0.2f));

    this->transparentRenderStage->draw(stateSet);
}



