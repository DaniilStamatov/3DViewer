#include <QApplication>
#include "UI/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;

    window.resize(800, 600);
    window.setWindowTitle("OpenGL Model Loader");

    window.LoadModel("cube.obj");

    window.show();
    return app.exec();
}