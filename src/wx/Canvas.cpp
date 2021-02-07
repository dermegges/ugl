/** @file Canvas.cpp



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

#include "ugl/wx/Canvas.hpp"

#include <wx/dcclient.h>
#include <wx/defs.h>

#include <algorithm>
#include <cctype>
#include <iostream>

namespace ugl
{
namespace wx
{

wxBEGIN_EVENT_TABLE( Canvas, wxGLCanvas )
EVT_SIZE( Canvas::resizeEvent )
EVT_PAINT( Canvas::paintGL )

EVT_LEFT_DOWN( Canvas::mousePressEvent )
EVT_RIGHT_DOWN( Canvas::mousePressEvent )
EVT_MIDDLE_DOWN( Canvas::mousePressEvent )
EVT_MOTION( Canvas::mouseMoveEvent )
EVT_LEFT_UP( Canvas::mouseReleaseEvent )
EVT_RIGHT_UP( Canvas::mouseReleaseEvent )
EVT_MIDDLE_UP( Canvas::mouseReleaseEvent )

EVT_KEY_DOWN( Canvas::keyPressEvent )
wxEND_EVENT_TABLE()

// -------------------------------------------------------------------------

Canvas::~Canvas()
{
}

// -------------------------------------------------------------------------

Canvas::Canvas(ugl::CoreApplication* application, wxWindow* parent ) :
    wxGLCanvas( parent, wxID_ANY, attribList ), application(application) ,
    m_wxGLContext( this ),
    m_glInitialized( false ),
    m_resized( true )
{
    SetBackgroundStyle( wxBG_STYLE_PAINT );
}


// -------------------------------------------------------------------------

void Canvas::initializeGL()
{
    SetCurrent( m_wxGLContext );
    this->application->uglInit();
}

// -------------------------------------------------------------------------

void Canvas::resizeGL()
{
    int width, height;
    GetClientSize( &width, &height );

    SetCurrent( m_wxGLContext );
    this->application->uglResize(glm::ivec2(width, height), glm::ivec2(width, height));
}

// -------------------------------------------------------------------------

void Canvas::resizeEvent( wxSizeEvent& /*event*/ )
{
    m_resized = true;
    Update();
}

// -------------------------------------------------------------------------

void Canvas::paintGL( wxPaintEvent& /*event*/ )
{
    if (this->application->isExitRequested())
        wxTheApp->GetTopWindow()->Close();

    SetCurrent( m_wxGLContext );
    wxPaintDC( this );

    if( !m_glInitialized )
    {
        initializeGL();
        m_glInitialized = true;
    }

    if( m_resized )
    {
        resizeGL();
        m_resized = false;
    }

    this->application->uglDraw();

    glFlush();
    SwapBuffers();
}

// -------------------------------------------------------------------------

void Canvas::mousePressEvent( wxMouseEvent& event )
{
    this->application->uglOnMousePress(event.GetX(), event.GetY(), event.LeftDown(), event.MiddleDown(), event.RightDown(), event.ShiftDown(), event.ControlDown(), event.AltDown());
    this->Refresh();

    event.Skip();
}


// -------------------------------------------------------------------------

void Canvas::mouseMoveEvent( wxMouseEvent& event )
{
    this->application->uglOnMouseMove(event.GetX(), event.GetY());
    this->Refresh();

    event.Skip();
}

// -------------------------------------------------------------------------

void Canvas::mouseReleaseEvent( wxMouseEvent& event )
{    
    this->application->uglOnMouseRelease(event.LeftUp(), event.MiddleUp(), event.RightUp());
    this->Refresh();

    event.Skip();
}

// -------------------------------------------------------------------------

void Canvas::keyPressEvent( wxKeyEvent& event )
{
    const int keyCode = event.GetKeyCode();

    this->application->uglOnKeyPress(keyCode, event.ShiftDown(), event.ControlDown(), event.AltDown());
    this->Refresh();

    event.Skip();
}


// -------------------------------------------------------------------------

// Core profile is available since wxWidgets 3.1.0
#if wxCHECK_VERSION(3, 1, 0)
const int Canvas::attribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_MIN_ALPHA, 8, WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 4, WX_GL_MINOR_VERSION, 0, 0 };
#else
const int Canvas::attribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_MIN_ALPHA, 8, 0 };
#endif

} // namespace wx
} // namespace ugl
