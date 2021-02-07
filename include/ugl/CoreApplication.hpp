/** @file CoreApplication.hpp



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

#ifndef COREAPPLICATION_HPP
#define COREAPPLICATION_HPP

#include "ViewController3D.hpp"

#include <glm/glm.hpp>


namespace ugl
{

/**
 * @brief Base class used for all ugl applications.
 */
class CoreApplication
{
public:
    CoreApplication();
    virtual ~CoreApplication();

    void uglInit();
    void uglDraw();
    void uglResize(const glm::ivec2& renderSize, const glm::ivec2& windowSize);

    void uglOnMousePress(int x, int y, bool left, bool middle, bool right, bool shift, bool control, bool alt);
    void uglOnMouseMove(int x, int y);
    void uglOnMouseRelease(bool left, bool middle, bool right);

    void uglOnKeyPress(int code, bool shift, bool control, bool alt); // TODO: need some platform/framework independent key codes  (maybe copy TwKeySpecial + ASCII codes style)
    void uglOnKeyRelease(int code);

    ViewController *getViewController() const; //! return the currently set view controller
    void setViewController(ViewController *viewController); //! set the view controller. The old view controller will be destroyed.
    glm::ivec2 getRenderSize() const;
    glm::ivec2 getWindowSize() const;

    void exit();
    bool isExitRequested();

protected:
    virtual void initGL() {}
    virtual void drawGL() {}
    virtual void resizeGL(int /*width*/, int /*height*/) {}

    virtual bool onMousePress(int /*x*/, int /*y*/, bool /*left*/, bool /*middle*/, bool /*right*/, bool /*shift*/, bool /*control*/, bool /*alt*/) { return false; }
    virtual bool onMouseMove(int /*x*/, int /*y*/) { return false; }
    virtual bool onMouseRelease(bool /*left*/, bool /*middle*/, bool /*right*/) { return false; }

    virtual bool onKeyPress(int /*code*/, bool /*shift*/, bool /*control*/, bool /*alt*/) { return false; }
    virtual bool onKeyRelease(int /*code*/) { return false; }

private:
    ViewController *viewController;
    glm::ivec2 renderSize;
    glm::ivec2 windowSize;
    bool exitRequested;
};

}

#endif // COREAPP_H
