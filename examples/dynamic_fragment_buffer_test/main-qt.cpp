#include "DFBTest.hpp"

#include <ugl/qt/Widget.hpp>

#include <QApplication>
#include <QMainWindow>


int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Dynamic Fragment Buffer Test (Qt)");
    window.setCentralWidget(new ugl::qt::Widget(&window, new DFBTest(), ugl::qt::OpenGLVersion(4,0)));
    window.resize(1024, 768);
    window.show();

    return application.exec();
}