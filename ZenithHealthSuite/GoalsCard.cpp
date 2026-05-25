#include "GoalsCard.h"
GoalsCard::GoalsCard(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}


GoalsCard::~GoalsCard()
{
	// we dont need dlete ui bcz it is not  pointer
}

void GoalsCard::setGoalData(const QString& targetName, const QString& category, double baseline, double threshold)
{

	ui.lblTargetName->setText(targetName);
	ui.lblCategory->setText(category.toUpper());

	ui.lblTargetDetails->setText("Target: " + QString::number(threshold));
	ui.lblCurrentStatus->setText("CURRENT STATUS: " + QString::number(baseline));

	int percentage = 0;
	if (threshold > 0) {
		percentage = (baseline / threshold) * 100;
	}

	if (percentage > 100) {
		percentage = 100;
	}

	ui.progressBar->setValue(percentage);
	ui.lblPercentage->setText(QString::number(percentage) + "% ACHIEVED");
}