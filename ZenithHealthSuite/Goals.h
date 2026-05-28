#pragma once
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>
#include "ui_Goals.h"
#include "DatabaseManager.h"
#include "Activity.h"
#include "History.h"
#include "analytics.h"
#include "DashboardScreen.h"
#include "user.h" 

class Goals : public QMainWindow
{
	Q_OBJECT

public:

	Goals(const user& currentUserObj, QWidget* parent = nullptr);
	~Goals();

private slots:
	void on_dashboardButton_clicked();
	void on_analyticsButton_clicked();
	void on_activityButton_clicked();
	void on_goalsButton_clicked();
	void on_historyButton_clicked();
	void on_settingsButton_clicked();
	void on_supportButton_clicked();
	void on_profileButton_clicked();
	void on_bellButton_clicked();

	void onInitializeTargetClicked();

	void on_syncDataButton_clicked();
	void setFrequency(QString freq);

private:
	Ui::GoalsClass ui;
	DatabaseManager dbManager;
	user loggedInUser; 
	QVBoxLayout* cardsLayout;

	void setupScrollArea();
	void loadGoalsFromDatabase();
	void clearForm();
	QString getSelectedFrequency();

	QString currentFrequency = "Daily";
};