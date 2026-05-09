#pragma once

#include <QMainWindow>
#include "ui_DashboardScreen.h"

class DashboardScreen : public QMainWindow
{
	Q_OBJECT

public:
	DashboardScreen(QWidget *parent = nullptr);
	~DashboardScreen();

private:
	Ui::DashboardScreen ui;
};

