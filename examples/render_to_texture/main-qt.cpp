#include "RenderToTexture.hpp"

#include <ugl/qt/Widget.hpp>

#include <QApplication>
#include <QMainWindow>


int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Render To Texture (Qt)");
    window.setCentralWidget(new ugl::qt::Widget(&window, new RenderToTexture(), ugl::qt::OpenGLVersion(4,0)));
    window.resize(960, 540);
    window.show();

    return application.exec();
}
