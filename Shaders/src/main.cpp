#include <QApplication>
#include "ShaderWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ShaderWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}