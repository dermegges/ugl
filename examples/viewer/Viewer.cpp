#include "Viewer.hpp"
#include "SkullVolume.hpp"

#include <ugl/Utils.hpp>
#include <ugl/ErrorCheck.hpp>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>

Viewer::Viewer() : surfaceRenderer(0), volume(0), transparentRenderStage(0), numSamples(0), showTweakBar(true), tweakBarHasMouse(false)
{
}


Viewer::~Viewer()
{
    TwTerminate();

    delete transparentRenderStage;
    delete surfaceRenderer;
    delete volume;
}

void Viewer::initGL()
{
    /*
     * Init globals and tweakbar
     */
    lightDirection  = glm::normalize( glm::vec3( 0.1, 0.1, -1 ) );
    lightGlossiness = 16.0;
    defaultColor    = glm::vec3( 1.0, 0.8, 0.2 );


    TwInit( TW_OPENGL_CORE, NULL );

    TwBar* bar = TwNewBar( "Globals" );
    TwAddVarRW( bar, "lightdi", TW_TYPE_DIR3F,   glm::value_ptr(lightDirection),  "label='Light Direction'" );
    TwAddVarRW( bar, "lightgl", TW_TYPE_FLOAT,   &lightGlossiness,                "label='Light Glossiness' min=0.1 max=100, step=0.1" );
    TwAddVarRW( bar, "defcol",  TW_TYPE_COLOR3F, glm::value_ptr(defaultColor),    "label='Default Color'" );

    TwAddSeparator(bar, "sep", 0);

    int maxSamples;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    std::stringstream ss;
    ss << "min=0 max=" << maxSamples << " label='Samples'";
    TwAddVarRW(bar, "samples", TW_TYPE_UINT32, &this->numSamples, ss.str().c_str());

    TwAddSeparator(bar, "sep2", 0);

    this->volumeRenderingEnabled = false;
    this->volumeRenderingSampleStepLength = 0.001f;
    TwAddVarRW(bar, "vrEnabled", TW_TYPE_BOOL8, &this->volumeRenderingEnabled, "label='Show Volume'");
    TwAddVarRW(bar, "vrStepSize", TW_TYPE_FLOAT, &this->volumeRenderingSampleStepLength, "label='Step Size' min=0.0001 max=0.01 step=0.0005");



    /*
     * Load data
     */
    std::string dataPath = ugl::getBaseDir() + "/data/uav.shear.90000";
    SurfaceVectorData svd(dataPath.c_str());
    surfaceRenderer = new SurfaceRenderer( svd );

    // Use bounding box to set reasonable home for the ViewController.
    ugl::BoundingBox box = surfaceRenderer->getBoundingBox();
    ugl::ViewController& viewCtrl = *(getViewController());
    viewCtrl.init(box);
    viewCtrl.reset();

    this->transparentRenderStage = new ugl::TransparentRenderStage();
    this->transparentRenderStage->addDrawable( surfaceRenderer );




    /*
     * Volume rendering test
     */
    box.add(glm::vec3(0, 0,  0.2));
    box.add(glm::vec3(0, 0, -0.2));

    this->volume = new SkullVolume(box, ugl::getBaseDir() + "/data/head256.raw", ugl::getBaseDir() + "/data/tff.dat");
    this->transparentRenderStage->addVolume(this->volume);
}


void Viewer::drawGL()
{
    const ugl::ViewController& viewCtrl = *(getViewController());

    glm::ivec2 size = this->getRenderSize();
    stateSet.getOrCreateViewport() = glm::uvec4(0u, 0u, size.x, size.y);

    ugl::UniformSet& uniforms = stateSet.getOrCreateUniforms();


    glm::mat4 projectionMatrix = viewCtrl.projection();
    glm::mat4 modelviewMatrix = viewCtrl.transform();

    uniforms.set( "globals.projectionMatrix", projectionMatrix );
    uniforms.set( "globals.modelviewMatrix",  modelviewMatrix );
    uniforms.set( "globals.normalMatrix",     viewCtrl.rotation() );

    uniforms.set( "globals.projectionMatrixInverse", glm::inverse(projectionMatrix));
    uniforms.set( "globals.modelviewMatrixInverse",  glm::inverse(modelviewMatrix));

    uniforms.set("globals.viewport", glm::ivec4(0, 0, size.x, size.y));

    uniforms.set( "globals.light[0].direction",  lightDirection );
    uniforms.set( "globals.light[0].glossiness", lightGlossiness );

    uniforms.set( "globals.defaultColor", defaultColor );

    if (this->transparentRenderStage->getNumSamples() != this->numSamples)
        this->transparentRenderStage->setNumSamples(this->numSamples);

    this->volume->setVisible(this->volumeRenderingEnabled);
    this->volume->setSampleStepSize(this->volumeRenderingSampleStepLength);

    transparentRenderStage->draw( stateSet );

    // Draw TweakBar
    if (this->showTweakBar)
        TwDraw();
}


void Viewer::resizeGL(int width, int height)
{
    // Resize TweakBar
    TwWindowSize(width, height);
}


bool Viewer::onMousePress(int /*x*/, int /*y*/, bool left, bool middle, bool right, bool /*shift*/, bool /*control*/, bool /*alt*/)
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


bool Viewer::onMouseMove(int x, int y)
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

        bool handled = TwMouseMotion( mx, my );
        this->tweakBarHasMouse = handled;

        return true;
    }

    return false;
}


bool Viewer::onMouseRelease(bool left, bool middle, bool right)
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


bool Viewer::onKeyPress(int code, bool shift, bool control, bool alt)
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


