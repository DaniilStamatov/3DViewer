#include <QApplication>
#include <QFontDatabase>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFont globalFont("SourceCodeVF", 11);
    QApplication::setFont(globalFont);
    MainWindow window;

    window.resize(1920, 1080);
    window.setWindowTitle("OpenGL Model Loader");

    window.show();
    return app.exec();
}