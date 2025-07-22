#include <QApplication>
#include "TransformationWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    TransformationWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}