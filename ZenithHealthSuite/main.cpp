#include "ZenithHealthSuite.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ZenithHealthSuite window;
    window.show();
    return app.exec();
}
