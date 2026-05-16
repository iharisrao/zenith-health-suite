#include "Activity.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

   
    Activity w;
    w.show();

    return a.exec();
}