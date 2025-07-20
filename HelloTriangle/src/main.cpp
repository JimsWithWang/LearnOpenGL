#include <QApplication>
#include "TriangleWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    TriangleWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}