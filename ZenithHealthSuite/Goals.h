#pragma once

#include <QMainWindow>
#include "ui_Goals.h"

class Goals : public QMainWindow
{
	Q_OBJECT

public:
	Goals(QWidget *parent = nullptr);
	~Goals();

private:
	Ui::GoalsClass ui;
};

