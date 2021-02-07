/** @file CoreApplication.cpp



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

#include "ugl/CoreApplication.hpp"

#include <GL/glew.h>
#include <iostream>
#include <algorithm>


namespace ugl
{

/**
 * @brief Constructor.
 */
CoreApplication::CoreApplication():viewController(new ViewController3D())
{
    this->exitRequested = false;
}


/**
 * @brief Destructor.
 */
CoreApplication::~CoreApplication()
{
    if(this->viewController)
        delete this->viewController;
}


/**
 * @brief Initialization of ugl/OpenGL.
 */
void CoreApplication::uglInit()
{
    // Initialize GLEW
    glewExperimental = true;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;

    glGetError();

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;

    // Do user initialization
    initGL();
}


/**
 * @brief Rendering.
 */
void CoreApplication::uglDraw()
{
    // Clear error flags
    while (glGetError() != GL_NO_ERROR);

    // User rendering
    drawGL();
}


/**
 * @brief Resize handler.
 * @param width
 * @param height
 */
void CoreApplication::uglResize(const glm::ivec2 &renderSize, const glm::ivec2 &windowSize)
{
    this->renderSize = renderSize;
    this->windowSize = windowSize;

    glViewport(0, 0, renderSize.x, renderSize.y);
    //this->viewController->resize(renderSize.x, renderSize.y);
    this->viewController->resize(windowSize.x, windowSize.y);

    // User resize handler
    resizeGL(renderSize.x, renderSize.y);
}


/**
 * @brief Mouse press event handler.
 * @param x
 * @param y
 * @param left
 * @param middle
 * @param right
 * @param shift
 * @param control
 * @param alt
 */
void CoreApplication::uglOnMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt)
{
    if (!onMousePress(x, y, left, middle, right, shift, control, alt))
    {
        // Trigger the view controller
        this->viewController->onMousePress(x, y, left, middle, right, shift, control, alt);
    }
}


/**
 * @brief Mouse move event handler.
 * @param x
 * @param y
 */
void CoreApplication::uglOnMouseMove(int x, int y)
{
    if (!onMouseMove(x, y))
    {
        // If the view controller is active, let it respond
        this->viewController->onMouseMove(x, y);
    }
}


/**
 * @brief Mouse release event handler.
 * @param left
 * @param middle
 * @param right
 */
void CoreApplication::uglOnMouseRelease(bool left, bool middle, bool right)
{
    if (!onMouseRelease(left, middle, right))
    {
        // End view controller
        this->viewController->onMouseRelease(left,middle,right);
    }
}


/**
 * @brief Key down event handler.
 * @param code
 * @param shift
 * @param control
 * @param alt
 */
void CoreApplication::uglOnKeyPress(int code, bool shift, bool control, bool alt)
{
    if (!onKeyPress(code, shift, control, alt))
    {
        // Escape
        if(code == 0x01000000 || code == 27)
            this->exit();

        // Space
        if(code == 0x20)
        {
            this->viewController->reset();
        }
    }
}


/**
 * @brief Key up event handler.
 * @param code
 */
void CoreApplication::uglOnKeyRelease(int code)
{
    if (!onKeyRelease(code))
    {}
}


/**
 * @brief Returns the current view controller.
 * @return
 */
ViewController *CoreApplication::getViewController() const
{
    return this->viewController;
}

/**
 * @brief Replaces the current view controller.
 * @return
 */
void CoreApplication::setViewController(ViewController * newViewController)
{
    if(this->viewController)
        delete this->viewController;

    this->viewController = newViewController;
    this->viewController->resize(renderSize.x, renderSize.y);
}

/**
 * @brief Returns the OpenGL render size since last resizeGL call.
 * @return
 */
glm::ivec2 CoreApplication::getRenderSize() const 
{
    return this->renderSize;
}


/**
 * @brief Returns the window size since last resizeGL call.
 * @return
 */
glm::ivec2 CoreApplication::getWindowSize() const
{
    return this->windowSize;
}


/**
 * @brief Sets the exit requested flag.
 */
void CoreApplication::exit()
{
    this->exitRequested = true;
}


/**
 * @brief Checks if the exit requested flag is set.
 * @return
 */
bool CoreApplication::isExitRequested()
{
    return this->exitRequested;
}

}
