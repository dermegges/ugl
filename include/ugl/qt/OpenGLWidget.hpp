/** @file OpenGLWidget.hpp



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

#ifndef UGL_OPENGL_WIDGET_H
#define UGL_OPENGL_WIDGET_H

#include "ugl/CoreApplication.hpp"

#include <GL/glew.h>
#include <QOpenGLWidget>

namespace ugl
{
namespace qt
{

/**
 * A Qt Widget for setting up the OpenGL context and passing input to the
 * ViewController3D.
 *
 * The equivalent for xwWidgets is GLCanvas.
 */
class OpenGLWidget : public QOpenGLWidget
{
//    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent, ugl::CoreApplication* application);
    virtual ~OpenGLWidget();   

    ugl::CoreApplication* getApplication() { return this->application; }
    const ugl::CoreApplication* getApplication() const { return this->application; }     

signals:
    void frameFinished();

protected:
    virtual void initializeGL();
    virtual void resizeGL( int w, int h );
    virtual void paintGL();

    virtual void mousePressEvent( QMouseEvent* e );
    virtual void mouseReleaseEvent( QMouseEvent* e );
    virtual void mouseMoveEvent( QMouseEvent* e );

    virtual void keyPressEvent( QKeyEvent* e );
    virtual void keyReleaseEvent( QKeyEvent* e );

private:
    ugl::CoreApplication* application;
};


} // namespace qt
} // namespace ugl
#endif // UGL_OPENGL_WIDGET_H