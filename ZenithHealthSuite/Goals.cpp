#include "Goals.h"
#include "GoalsCard.h"
#include <QMessageBox>

Goals::Goals(const QString& username, QWidget* parent)
	: QMainWindow(parent), currentUsername(username)
{
	ui.setupUi(this);
	this->showMaximized();
	this->setWindowTitle("ZenithHealthSuite - Goals");

	ui.comboCategory->clear(); 
	ui.comboCategory->addItem("Biometric Data");
	ui.comboCategory->addItem("Performance Metric");
	ui.comboCategory->addItem("Nutritional Target");
	ui.comboCategory->addItem("Mental Health");



	connect(ui.btnInitializeTarget, &QPushButton::clicked, this, &Goals::onInitializeTargetClicked);

	setupScrollArea();
	loadGoalsFromDatabase();
}

Goals::~Goals()
{
}

void Goals::on_dashboardButton_clicked() {
	DashboardScreen* dashboard = new DashboardScreen();
	dashboard->show();
	this->close();
}

void Goals::on_analyticsButton_clicked() {
	analytics* analyticsScreen = new analytics();
	analyticsScreen->show();
	this->close();
}

void Goals::on_activityButton_clicked() {
	Activity* activityScreen = new Activity();
	activityScreen->show();
	this->close();
}

void Goals::on_goalsButton_clicked() {
	qDebug() << "Already on the Goals!";
}

void Goals::on_historyButton_clicked() {
	HistoryScreen* historyScreen = new HistoryScreen();
	historyScreen->show();
	this->close();
}

void Goals::on_settingsButton_clicked() {
	qDebug() << "Settings coming soon...";
}

void Goals::on_supportButton_clicked() {
	qDebug() << "Support coming soon...";
}

void Goals::on_profileButton_clicked() {
	qDebug() << "User profile...";
}

void Goals::on_bellButton_clicked() {
	qDebug() << "No new notifications.";
}




void Goals::setupScrollArea()
{
	QWidget* containerWidget = new QWidget();
	cardsLayout = new QVBoxLayout(containerWidget);

	cardsLayout->setAlignment(Qt::AlignTop);
	cardsLayout->setSpacing(15);
	cardsLayout->setContentsMargins(10, 10, 10, 10);

	ui.scrollAreaGoals->setWidget(containerWidget);
	ui.scrollAreaGoals->setWidgetResizable(true);
}

void Goals::loadGoalsFromDatabase()
{
	QLayoutItem* item;
	while ((item = cardsLayout->takeAt(0)) != nullptr) {
		delete item->widget();
		delete item;
	}

	QList<GoalRecord> goalsList = dbManager.getGoals(currentUsername);

	int activeGoalsCount = goalsList.size();
	double totalPercentage = 0;

	for (const GoalRecord& record : goalsList) {
		GoalsCard* card = new GoalsCard(this);

		card->setGoalData(record.targetName, record.category, record.baseline, record.threshold);
		cardsLayout->addWidget(card);

		if (record.threshold > 0) {
			double pct = (record.baseline / record.threshold) * 100.0;
			if (pct > 100) pct = 100;
			totalPercentage += pct;
		}
	}

	ui.lblActiveGoalsValue->setText(QString("%1").arg(activeGoalsCount, 2, 10, QChar('0')));

	double avgCompletion = (activeGoalsCount > 0) ? (totalPercentage / activeGoalsCount) : 0;
	ui.lblCompletionRateValue->setText(QString::number(static_cast<int>(avgCompletion)) + "%");
}

void Goals::onInitializeTargetClicked()
{
	QString targetName = ui.txtTargetName->text();
	QString category = ui.comboCategory->currentText();
	QString baselineStr = ui.txtBaseline->text();
	QString thresholdStr = ui.txtThreshold->text();
	QString frequency = getSelectedFrequency();

	if (targetName.isEmpty() || baselineStr.isEmpty() || thresholdStr.isEmpty() || frequency.isEmpty()) {
		QMessageBox::warning(this, "Missing Data", "Please fill out all fields before initializing the target.");
		return;
	}

	double baseline = baselineStr.toDouble();
	double threshold = thresholdStr.toDouble();

	bool success = dbManager.saveGoal(currentUsername, targetName, category, baseline, threshold, frequency);

	if (success) {
		clearForm();
		loadGoalsFromDatabase();
	}
	else {
		QMessageBox::critical(this, "Database Error", "Failed to save the new goal.");
	}
}

QString Goals::getSelectedFrequency()
{
	if (ui.btnDaily->isChecked()) return "Daily";
	if (ui.btnWeekly->isChecked()) return "Weekly";
	if (ui.btnQuarterly->isChecked()) return "Quarterly";

	return "Daily";
}

void Goals::clearForm()
{
	ui.txtTargetName->clear();
	ui.txtBaseline->clear();
	ui.txtThreshold->clear();
	ui.comboCategory->setCurrentIndex(0);
	ui.btnDaily->setChecked(true);
}