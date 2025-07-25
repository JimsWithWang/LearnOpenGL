#include <QApplication>
#include "CoordinateWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    CoordinateWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}