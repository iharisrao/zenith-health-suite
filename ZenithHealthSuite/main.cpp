#include "analytics.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

   
    analytics w;
    w.show();

    return a.exec();
}