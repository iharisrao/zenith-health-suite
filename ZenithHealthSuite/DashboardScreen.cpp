#include "DashboardScreen.h"
#include "ui_DashboardScreen.h"
#include "DataEntryDialog.h" 

DashboardScreen::DashboardScreen(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardScreen)
{
    ui->setupUi(this);
    this->showMaximized();
}

DashboardScreen::~DashboardScreen()
{
    delete ui;
}

void DashboardScreen::on_addActivityButton_clicked()
{
    qDebug() << "Launching Data Entry Pop-up...";
    DataEntryDialog popUpWindow(this);
    popUpWindow.exec(); 
}


void DashboardScreen::on_dashboardButton_clicked() {
    qDebug() << "You are already on the Dashboard!";
}

void DashboardScreen::on_analyticsButton_clicked() {
    qDebug() << "Analytics screen coming soon...";
    // Later Amna will add: AnalyticsScreen window(this); window.exec();
}

void DashboardScreen::on_activityButton_clicked() {
    qDebug() << "Activity log screen coming soon...";
}

void DashboardScreen::on_goalsButton_clicked() {
    qDebug() << "Goals screen coming soon...";
}

void DashboardScreen::on_historyButton_clicked() {
    qDebug() << "History screen coming soon...";
}

void DashboardScreen::on_settingsButton_clicked() {
    qDebug() << "Settings screen coming soon...";
}

void DashboardScreen::on_supportButton_clicked() {
    qDebug() << "Support screen coming soon...";
}

void DashboardScreen::on_buddyButton_clicked() {
    qDebug() << "Buddy AI screen coming soon...";
}

void DashboardScreen::on_premiumButton_clicked() {
    qDebug() << "Premium checkout coming soon...";
}

void DashboardScreen::on_profileButton_clicked() {
    qDebug() << "User profile coming soon...";
}

void DashboardScreen::on_bellButton_clicked() {
    qDebug() << "No new notifications.";
}

void DashboardScreen::updateDashboardMetrics(int calories, int steps, int heartRate)
{
    // buddy code will come here
}