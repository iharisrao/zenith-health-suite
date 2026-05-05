#pragma once

#include <QMainWindow>
#include "ui_RegistrationScreen.h"

class RegistrationScreen : public QMainWindow
{
	Q_OBJECT

public:
	RegistrationScreen(QWidget *parent = nullptr);
	~RegistrationScreen();

private:
	Ui::RegistrationScreenClass ui;
};

