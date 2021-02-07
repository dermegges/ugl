#include "BasicViewer.hpp"

#include <AntTweakBar.h>
#include <iostream>
#include <algorithm>
#include <sstream>


/**
 * @brief Constructor.
 */
BasicViewer::BasicViewer() : meshDrawable(0), transparentRenderStage(0), showTweakBar(true), tweakBarHasMouse(false), numSamples(4)
{
}


/**
 * @brief Destructor.
 */
BasicViewer::~BasicViewer()
{
    TwTerminate();

    delete transparentRenderStage;
    delete meshDrawable;
}


/**
 * @brief Initialization.
 */
void BasicViewer::initGL()
{
    // Create data
    ugl::MeshData data;

    // simple tetrahedron
    data.addPoint(glm::vec3( 0.0f, 0.0f, 0.0f ));
    data.addPoint(glm::vec3( 1.0f, 0.0f, 0.0f ));
    data.addPoint(glm::vec3( 0.0f, 1.0f, 0.0f ));
    data.addPoint(glm::vec3( 0.0f, 0.0f, 1.0f ));

    data.addTriangle(glm::uvec3( 0u, 2u, 1u ));
    data.addTriangle(glm::uvec3( 0u, 1u, 3u ));
    data.addTriangle(glm::uvec3( 0u, 3u, 2u ));
    data.addTriangle(glm::uvec3( 1u, 2u, 3u ));

    meshDrawable = new ugl::MeshDrawable( data );


    // Use bounding box to set reasonable home for the ViewController.
    ugl::BoundingBox box = meshDrawable->getBoundingBox();
    ugl::ViewController &viewCtrl = *(getViewController());
    viewCtrl.init(box);
    viewCtrl.reset();

    transparentRenderStage = new ugl::TransparentRenderStage();
    transparentRenderStage->addDrawable( meshDrawable );

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

    TwAddSeparator(globalsBar, "sep", 0);

    int maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    std::stringstream ss;
    ss << "min=0 max=" << maxSamples << " label='Samples'";
    TwAddVarRW(globalsBar, "samples", TW_TYPE_UINT32, &this->numSamples, ss.str().c_str());



    bool* drawMeshSurface;
    bool* drawMeshEdges;
    float* meshAlpha;
    meshDrawable->getTweakableParameters(&drawMeshSurface, &drawMeshEdges, &meshAlpha);

    TwBar* meshBar = TwNewBar( "Mesh" );
    TwAddVarRW( meshBar, "surface", TW_TYPE_BOOLCPP, drawMeshSurface, "label='Draw Surface' key=s" );
    TwAddVarRW( meshBar, "edges",   TW_TYPE_BOOLCPP, drawMeshEdges, "label='Draw Edges' key=e" );
    TwAddVarRW( meshBar, "alpha",   TW_TYPE_FLOAT,   meshAlpha, "label='Alpha' min=0.0 max=1.0 step=0.01" );
}


/**
 * @brief Rendering.
 */
void BasicViewer::drawGL()
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


    if (this->transparentRenderStage->getNumSamples() != this->numSamples)
        this->transparentRenderStage->setNumSamples(this->numSamples);

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
void BasicViewer::resizeGL(int width, int height)
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
bool BasicViewer::onMousePress(int /*x*/, int /*y*/, bool left, bool middle, bool right, bool /*shift*/, bool /*control*/, bool /*alt*/)
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
bool BasicViewer::onMouseMove(int x, int y)
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
bool BasicViewer::onMouseRelease(bool left, bool middle, bool right)
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
bool BasicViewer::onKeyPress(int code, bool shift, bool control, bool alt)
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



