#pragma once

#include <QMainWindow>
#include "ui_analytics.h"

class analytics : public QMainWindow
{
	Q_OBJECT

public:
	analytics(QWidget *parent = nullptr);
	~analytics();

private:
	Ui::analyticsClass ui;
};

