#include "RegistrationScreen.h" // 1. Bring in our Registration Blueprint
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    // 2. Tell the app to build and show our specific screen!
    RegistrationScreen w;
    w.show();

    return a.exec();
}