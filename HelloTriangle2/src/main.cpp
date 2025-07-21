#include <QApplication>
#include "RectangleWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    RectangleWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}