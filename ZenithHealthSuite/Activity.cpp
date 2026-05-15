#include "Activity.h"

Activity::Activity(QWidget *parent)
	: QMainWindow(parent)
{
	ui->setupUi(this);
	this->setWindowTitle("ZenithHealthSuite - Activity");
}

Activity::~Activity()
{}

