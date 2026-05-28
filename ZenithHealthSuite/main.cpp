#include "login.h"
#include <QtWidgets/QApplication>
#include <QIcon>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/icons/icons/app_icon.ico"));

    login w;
    w.show();

    return a.exec();
}