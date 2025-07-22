#include <QApplication>
#include "TextureWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    TextureWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}