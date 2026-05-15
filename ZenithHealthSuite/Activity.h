#pragma once

#include <QMainWindow>
#include "ui_Activity.h"

class Activity : public QMainWindow
{
	Q_OBJECT

public:
	Activity(QWidget *parent = nullptr);
	~Activity();

private:
	Ui::ActivityClass *ui;
};

