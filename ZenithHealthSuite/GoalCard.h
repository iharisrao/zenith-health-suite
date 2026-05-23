#pragma once

#include <QMainWindow>
#include "ui_GoalCard.h"

class GoalCard : public QMainWindow
{
	Q_OBJECT

public:
	GoalCard(QWidget *parent = nullptr);
	~GoalCard();

private:
	Ui::GoalCardClass ui;
};

