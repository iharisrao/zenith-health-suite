#include "GoalsCard.h"

GoalsCard::GoalsCard(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

GoalsCard::~GoalsCard()
{
}

void GoalsCard::setGoalData(const QString& targetName, const QString& category, double originalBaseline, double currentStatus, double threshold)
{
    ui.lblTargetName->setText(targetName);
    ui.lblCategory->setText(category);
    ui.lblTargetDetails->setText("Target: " + QString::number(threshold));

    ui.lblCurrentStatus->setText("CURRENT STATUS: " + QString::number(currentStatus));

    double percentage = 0.0;
    if (threshold != originalBaseline) {
        percentage = ((currentStatus - originalBaseline) / (threshold - originalBaseline)) * 100.0;
    }

    if (percentage > 100.0) percentage = 100.0;
    if (percentage < 0.0) percentage = 0.0;

    ui.progressBar->setValue(static_cast<int>(percentage));
    ui.progressBar->setMinimumHeight(10);
    ui.lblPercentage->setText(QString::number(static_cast<int>(percentage)) + "% ACHIEVED.");

    ui.lblTargetName->setStyleSheet("background: transparent; font-size: 15px; font-weight: 900; color: #0F172A;");
    ui.lblCategory->setStyleSheet("background: transparent; font-size: 11px; font-weight: bold; color: #64748B;");
    ui.lblTargetDetails->setStyleSheet("background: transparent; font-size: 12px; color: #8C98A4;");
    ui.lblCurrentStatus->setStyleSheet("background: transparent; font-size: 11px; font-weight: bold; color: #0F172A;");

    ui.lblPercentage->setStyleSheet("background: transparent; font-size: 12px; font-weight: bold; color: #1558A8;");

    ui.progressBar->setStyleSheet(
        "QProgressBar { border: none; border-radius: 5px; background-color: #E2E8F0; color: transparent; }"
        "QProgressBar::chunk { background-color: #1558A8; border-radius: 5px; }"
    );
}