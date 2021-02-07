/** @file Canvas.hpp



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

#ifndef _CANVAS_HPP_
#define _CANVAS_HPP_

#include "ugl/CoreApplication.hpp"

#include <wx/event.h>
#include <wx/glcanvas.h>
#include <wx/window.h>

namespace ugl
{
namespace wx
{

/**
 * A wxWidgets Window for setting up the OpenGL context and passing input to the
 * ViewController3D.
 *
 * The equivalent for Qt is GLWidget.
 */
class Canvas : public wxGLCanvas
{
public:
    Canvas(ugl::CoreApplication* application,  wxWindow* parent);
    virtual ~Canvas();

    ugl::CoreApplication* getApplication() { return this->application; }
    const ugl::CoreApplication* getApplication() const { return this->application; }

private:
    void initializeGL();
    void resizeGL();

    void resizeEvent( wxSizeEvent& event );
    void paintGL( wxPaintEvent& event );

    void mousePressEvent( wxMouseEvent& event );
    void mouseMoveEvent( wxMouseEvent& event );
    void mouseReleaseEvent( wxMouseEvent& event );

    void keyPressEvent( wxKeyEvent& event );


private:
    ugl::CoreApplication* application;

    wxGLContext         m_wxGLContext;
    bool                m_glInitialized;
    bool                m_resized;

    static const int    attribList[];

    wxDECLARE_EVENT_TABLE();
};

} // namespace wx
} // namespace ugl
#endif // _CANVAS_HPP_
