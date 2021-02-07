#include "DFBTest.hpp"
#include "SurfaceVectorData.hpp"

#include <AntTweakBar.h>
#include <iostream>
#include <algorithm>
#include <sstream>


/**
 * @brief Constructor.
 */
DFBTest::DFBTest() : transparentRenderStage(0), surfaceRenderer(0), showTweakBar(true), tweakBarHasMouse(false)
{
}


/**
 * @brief Destructor.
 */
DFBTest::~DFBTest()
{
    TwTerminate();

    delete transparentRenderStage;
    delete surfaceRenderer;
}


/**
 * @brief Initialization.
 */
void DFBTest::initGL()
{
    lightDirection  = glm::normalize(glm::vec3(0.1f, 0.1f, -1.0f));
    lightGlossiness = 16.0f;
    defaultColor    = glm::vec3(1.0f, 0.8f, 0.2f);


    /*
     * Init TweakBar
     */
    TwInit( TW_OPENGL_CORE, NULL );

    TwBar* globalsBar = TwNewBar("Globals");
    TwAddVarRW(globalsBar, "lightdi", TW_TYPE_DIR3F,   glm::value_ptr(lightDirection), "label='Light Direction'");
    TwAddVarRW(globalsBar, "lightgl", TW_TYPE_FLOAT,   &lightGlossiness, "label='Light Glossiness' min=0.1 max=100, step=0.1");
    TwAddVarRW(globalsBar, "defcol",  TW_TYPE_COLOR3F, glm::value_ptr(defaultColor), "label='Default Color'");


    // Load data
    std::string dataPath = ugl::getBaseDir() + "/data/uav.shear.90000";
    SurfaceVectorData svd(dataPath.c_str());
    surfaceRenderer = new SurfaceRenderer( svd );

    // Use bounding box to set reasonable home for the ViewController.
    ugl::BoundingBox box = surfaceRenderer->getBoundingBox();
    ugl::ViewController& viewCtrl = *(getViewController());
    viewCtrl.init(box);
    viewCtrl.reset();

    this->transparentRenderStage = new ugl::TransparentRenderStageDFB();  // Note: Dynamic Fragment Buffer (DFB) version is used
    this->transparentRenderStage->addDrawable(surfaceRenderer);
}


/**
 * @brief Rendering.
 */
void DFBTest::drawGL()
{
    const ugl::ViewController &viewCtrl = *(getViewController());

    glm::ivec2 size = this->getRenderSize();
    stateSet.getOrCreateViewport() = glm::uvec4(0u, 0u, size.x, size.y);

    ugl::UniformSet& uniforms = stateSet.getOrCreateUniforms();

    uniforms.set("globals.projectionMatrix", viewCtrl.projection());
    uniforms.set("globals.modelviewMatrix",  viewCtrl.transform());
    uniforms.set("globals.normalMatrix",     viewCtrl.rotation());

    uniforms.set("globals.light[0].direction",  lightDirection);
    uniforms.set("globals.light[0].glossiness", lightGlossiness);

    uniforms.set("globals.defaultColor", defaultColor);

    transparentRenderStage->draw(stateSet);


    // Draw TweakBar
    if (this->showTweakBar)
        TwDraw();
}


/**
 * @brief Resize handler.
 * @param width
 * @param height
 */
void DFBTest::resizeGL(int width, int height)
{
    // Resize TweakBar
    TwWindowSize(width, height);
}


/**
 * @brief Mouse press event handler.
 * @param left
 * @param middle
 * @param right
 * @return
 */
bool DFBTest::onMousePress(int /*x*/, int /*y*/, bool left, bool middle, bool right, bool /*shift*/, bool /*control*/, bool /*alt*/)
{
    // First, check if TweakBar responds to button
    if(this->showTweakBar && this->tweakBarHasMouse )
    {
        TwMouseButtonID button;

        if (left)
            button = TW_MOUSE_LEFT;
        else if (middle)
            button = TW_MOUSE_MIDDLE;
        else if (right)
            button = TW_MOUSE_RIGHT;
        else
            return false;

        if (TwMouseButton( TW_MOUSE_PRESSED, button ))
            return true;
    }

    return false;
}


/**
 * @brief Mouse move event handler.
 * @param x
 * @param y
 * @return
 */
bool DFBTest::onMouseMove(int x, int y)
{
    // Let the TweakBar handle it if view controller mode is none
    if(!(this->getViewController()->isInteracting()) && this->showTweakBar)
    {
        // OS X high-dpi / Retina mode:
        // TweakBar needs non-doubled coordinates
        glm::ivec2 renderSize = this->getRenderSize();
        glm::ivec2 windowSize = this->getWindowSize();

        int mx = x * (float)renderSize.x / (float)windowSize.x;
        int my = y * (float)renderSize.y / (float)windowSize.y;

        bool handled = TwMouseMotion( mx, my ) != 0;
        this->tweakBarHasMouse = handled;

        return true;
    }

    return false;
}


/**
 * @brief Mouse release event handler.
 * @param left
 * @param middle
 * @param right
 * @return
 */
bool DFBTest::onMouseRelease(bool left, bool middle, bool right)
{
    TwMouseButtonID button;

    if (left)
        button = TW_MOUSE_LEFT;
    else if (middle)
        button = TW_MOUSE_MIDDLE;
    else if (right)
        button = TW_MOUSE_RIGHT;
    else
        return false;

    if (TwMouseButton(TW_MOUSE_RELEASED, button))
        return true;

    return false;
}


/**
 * @brief Key press event handler.
 * @param code
 * @param shift
 * @param control
 * @param alt
 * @return
 */
bool DFBTest::onKeyPress(int code, bool shift, bool control, bool alt)
{
    // Tab toggles TweakBar
    if(code == 0x01000001)
    {
        this->showTweakBar = !this->showTweakBar;
        return true;
    }

    // Other tweakbar keys
    int mod = TW_KMOD_NONE;

    if(alt)
        mod |= TW_KMOD_ALT;
    if(shift)
        mod |= TW_KMOD_SHIFT;
    if(control)
        mod |= TW_KMOD_CTRL;

    if(TwKeyPressed( code, mod ))
        return true;

    return false;
}



