#pragma once
#include <QWidget>
#include "ui_GoalsCard.h"
#include <QString>

class GoalsCard : public QWidget
{
    Q_OBJECT
public:
    GoalsCard(QWidget* parent = nullptr);
    ~GoalsCard();
    void setGoalData(const QString& targetName, const QString& category, double originalBaseline, double currentStatus, double threshold);

private:
    Ui::GoalsCardClass ui;
};