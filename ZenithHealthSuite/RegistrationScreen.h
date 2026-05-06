#pragma once

#include <QMainWindow>
#include "ui_RegistrationScreen.h"

class RegistrationScreen : public QMainWindow
{
	Q_OBJECT

public:
	RegistrationScreen(QWidget *parent = nullptr);
	~RegistrationScreen();
private slots:
	void on_completeRegistration_Clicked();	

private:
	Ui::RegistrationScreenClass ui;
};

