#include "analytics.h"

analytics::analytics(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->showMaximized();
}

analytics::~analytics()
{
}

