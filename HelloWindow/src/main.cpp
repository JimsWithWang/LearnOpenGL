#include <QApplication>
#include "ClearWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    ClearWidget window;
    window.resize(960, 640);
    window.show();
    return app.exec();
}