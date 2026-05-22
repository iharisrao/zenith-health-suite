#include "DashboardScreen.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

   
    DashboardScreen w;
    w.show();

    return a.exec();
}