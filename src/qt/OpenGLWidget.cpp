/** @file OpenGLWidget.cpp



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

    Author(s): A.Lohfink
*/

#include "ugl/qt/OpenGLWidget.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QKeyEvent>

#include <iostream>


namespace ugl
{
namespace qt
{

/**
 * @brief Constructor.
 * @param parent
 * @param application
 * @param profile
 */
OpenGLWidget::OpenGLWidget(QWidget* parent, ugl::CoreApplication* application) : QOpenGLWidget(parent), application(application)
{
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //setFocusPolicy(Qt::StrongFocus);
}


/**
 * @brief Destructor.
 */
OpenGLWidget::~OpenGLWidget()
{
}


/**
 * @brief Initialization.
 */
void OpenGLWidget::initializeGL()
{
    this->application->uglInit();
    this->setMouseTracking(true);
}


/**
 * @brief Resize handler.
 * @param w
 * @param h
 */
void OpenGLWidget::resizeGL( int w, int h )
{
    this->makeCurrent();
    this->application->uglResize(glm::ivec2(w, h), glm::ivec2(width(), height()));
}


/**
 * @brief Drawing.
 */
void OpenGLWidget::paintGL()
{
    // Check if exit is requested
    if (this->application->isExitRequested())
        QCoreApplication::instance()->quit();

/*    this->makeCurrent();
    this->application->uglDraw();

    // Draw
    emit this->frameFinished();*/

    this->application->uglDraw();
}


/**
 * @brief Mouse press event handler.
 * @param e
 */
void OpenGLWidget::mousePressEvent( QMouseEvent* e )
{
    this->application->uglOnMousePress(e->x(), e->y(), e->buttons() & Qt::LeftButton, e->buttons() & Qt::MiddleButton, e->buttons() & Qt::RightButton, e->modifiers() & Qt::ShiftModifier, e->modifiers() & Qt::ControlModifier, e->modifiers() & Qt::AltModifier);
    this->update();
}


/**
 * @brief Mouse move event handler.
 * @param e
 */
void OpenGLWidget::mouseMoveEvent( QMouseEvent* e )
{
    this->application->uglOnMouseMove(e->x(), e->y());
    this->update();
}


/**
 * @brief Mouse release event handler.
 * @param e
 */
void OpenGLWidget::mouseReleaseEvent( QMouseEvent* e )
{
    this->application->uglOnMouseRelease(e->button() == Qt::LeftButton, e->button() == Qt::MiddleButton, e->button() == Qt::RightButton);
    this->update();
}


/**
 * @brief Key press event handler.
 * @param e
 */
void OpenGLWidget::keyPressEvent( QKeyEvent* e )
{
    this->application->uglOnKeyPress(e->key(), e->modifiers() & Qt::ShiftModifier, e->modifiers() & Qt::ControlModifier, e->modifiers() & Qt::AltModifier);
    this->update();

    QOpenGLWidget::keyPressEvent(e);
}


/**
 * @brief Key release event handler.
 * @param e
 */
void OpenGLWidget::keyReleaseEvent(QKeyEvent* e)
{
    this->application->uglOnKeyRelease(e->key());
    this->update();
}


} // namespace qt
} // namespace ugl
