#include "Activity.h"
#include "ui_Activity.h"

Activity::Activity(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ActivityClass) 
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("ZenithHealthSuite - Activity");
}

Activity::~Activity()
{
    delete ui;
}