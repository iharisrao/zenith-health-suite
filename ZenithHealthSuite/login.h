#pragma once

#include <QMainWindow>
#include "ui_login.h"

class login : public QMainWindow
{
	Q_OBJECT

public:
	login(QWidget *parent = nullptr);
	~login();

private:
	Ui::loginClass ui;
};

