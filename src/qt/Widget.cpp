/** @file Widget.cpp



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

#include "ugl/qt/Widget.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>

#include <iostream>


namespace ugl
{
namespace qt
{

// -------------------------------------------------------------------------

void printGLError( const char* file, const char* line )
{
    GLenum error = glGetError();

    if( error == GL_NO_ERROR )
        return;

    const char* which = "UNKNOWN";

    switch( error )
    {
    case GL_INVALID_ENUM: which = "GL_INVALID_ENUM"; break;
    case GL_INVALID_VALUE: which = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: which = "GL_INVALID_OPERATION"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: which = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    case GL_OUT_OF_MEMORY: which = "GL_OUT_OF_MEMORY"; break;
    case GL_STACK_UNDERFLOW: which = "GL_STACK_UNDERFLOW"; break;
    case GL_STACK_OVERFLOW: which = "GL_STACK_OVERFLOW"; break;
    }

    fprintf( stderr, "OpenGL error %s detected at %s:%s\n", which, file, line );
}

#define checkGLError do { printGLError( __FILE__, __LINE__ ); }

// -------------------------------------------------------------------------

static QGLFormat defaultFormat(const OpenGLVersion& profile)
{
    QGLFormat fmt;

    if (profile.isValid())
    {
        fmt.setProfile(QGLFormat::CoreProfile); // Requires >= Qt 4.7
        fmt.setVersion(profile.getMajorVersion(), profile.getMinorVersion());
    }

    return fmt;
}

// -------------------------------------------------------------------------


/**
 * @brief Constructor.
 * @param parent
 * @param application
 * @param profile
 */
Widget::Widget(QWidget* parent, ugl::CoreApplication* application, const OpenGLVersion& profile) : QGLWidget(defaultFormat(profile), parent), application(application)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
}


/**
 * @brief Destructor.
 */
Widget::~Widget()
{
}


/**
 * @brief Initialization.
 */
void Widget::initializeGL()
{
    this->makeCurrent();
    this->application->uglInit();

    this->setMouseTracking(true);
}


/**
 * @brief Resize handler.
 * @param w
 * @param h
 */
void Widget::resizeGL( int w, int h )
{
    this->makeCurrent();
    this->application->uglResize(glm::ivec2(w, h), glm::ivec2(width(), height()));
}


/**
 * @brief Drawing.
 */
void Widget::paintGL()
{
    // Check if exit is requsted
    if (this->application->isExitRequested())
        QCoreApplication::instance()->quit();

    this->makeCurrent();
    this->application->uglDraw();

    // Draw
    emit this->frameFinished();
}


/**
 * @brief Mouse press event handler.
 * @param e
 */
void Widget::mousePressEvent( QMouseEvent* e )
{
    this->application->uglOnMousePress(e->x(), e->y(), e->buttons() & Qt::LeftButton, e->buttons() & Qt::MiddleButton, e->buttons() & Qt::RightButton, e->modifiers() & Qt::ShiftModifier, e->modifiers() & Qt::ControlModifier, e->modifiers() & Qt::AltModifier);
    this->update();
}


/**
 * @brief Mouse move event handler.
 * @param e
 */
void Widget::mouseMoveEvent( QMouseEvent* e )
{
    this->application->uglOnMouseMove(e->x(), e->y());
    this->update();
}


/**
 * @brief Mouse release event handler.
 * @param e
 */
void Widget::mouseReleaseEvent( QMouseEvent* e )
{
    this->application->uglOnMouseRelease(e->button() == Qt::LeftButton, e->button() == Qt::MiddleButton, e->button() == Qt::RightButton);
    this->update();
}


/**
 * @brief Key press event handler.
 * @param e
 */
void Widget::keyPressEvent( QKeyEvent* e )
{
    this->application->uglOnKeyPress(e->key(), e->modifiers() & Qt::ShiftModifier, e->modifiers() & Qt::ControlModifier, e->modifiers() & Qt::AltModifier);
    this->update();

    QGLWidget::keyPressEvent(e);
}


/**
 * @brief Key release event handler.
 * @param e
 */
void Widget::keyReleaseEvent(QKeyEvent* e)
{
    this->application->uglOnKeyRelease(e->key());
    this->update();
}


} // namespace qt
} // namespace ugl
