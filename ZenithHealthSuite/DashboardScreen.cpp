#include "DashboardScreen.h"
#include "ui_DashboardScreen.h"
#include "DataEntryDialog.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QVBoxLayout>
#include <QDate>


DashboardScreen::DashboardScreen(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardScreen)
{
    ui->setupUi(this);
    this->showMaximized();

	setupHeartRateChart();
    setupActiveEnergyChart();
   
	updateDashboardMetrics(1840, 2400, 9248, 742);
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
    qDebug() << "Already on the Dashboard!";
}

void DashboardScreen::on_analyticsButton_clicked() {
    qDebug() << "Analytics coming soon...";
}

void DashboardScreen::on_activityButton_clicked() {
    qDebug() << "Activity log coming soon...";
}

void DashboardScreen::on_goalsButton_clicked() {
    qDebug() << "Goals coming soon...";
}

void DashboardScreen::on_historyButton_clicked() {
    qDebug() << "History coming soon...";
}

void DashboardScreen::on_settingsButton_clicked() {
    qDebug() << "Settings coming soon...";
}

void DashboardScreen::on_supportButton_clicked() {
    qDebug() << "Support coming soon...";
}

void DashboardScreen::on_buddyButton_clicked() {
    qDebug() << "Buddy AI coming soon...";
}

void DashboardScreen::on_premiumButton_clicked() {
    qDebug() << "Premium checkout...";
}

void DashboardScreen::on_profileButton_clicked() {
    qDebug() << "User profile...";
}

void DashboardScreen::on_bellButton_clicked() {
    qDebug() << "No new notifications.";
}

void DashboardScreen::updateDashboardMetrics(int caloriesConsumed, int caloriesGoal, int steps, int activeEnergy)
{
    if (caloriesGoal == 0) caloriesGoal = 1;
    int caloriesPercentage = (static_cast<double>(caloriesConsumed) / caloriesGoal) * 100;
    int stepsPercentage = (static_cast<double>(steps) / 10000.0) * 100;
    if (stepsPercentage > 100) stepsPercentage = 100;

    ui->caloriesProgressBar->setValue(caloriesPercentage);
    ui->stepCountProgressBar->setValue(stepsPercentage);

    QLocale locale(QLocale::English);

    ui->caloriesTextLabel->setText(locale.toString(caloriesConsumed));
    ui->stepCountValueLabel->setText(locale.toString(steps));
    ui->activeEnergyValueLabel->setText(locale.toString(activeEnergy));
}
void DashboardScreen::setupHeartRateChart()
{
	if (!ui->heartRateChartBox) return;
	QLineSeries* series = new QLineSeries();
	series->append(0, 60);series->append(1, 60);series->append(1.5,75);
    series->append(1.8, 45);series->append(2.2,85);series->append(2.5,40);
    series->append(3,62);series->append(4,62);series->append(4.5,70);
    series->append(4.8,55);series->append(5.2, 64);series->append(6,64);

	QPen pen(QColor("#D32F2F"));
	pen.setWidth(3);
	series->setPen(pen);

	QChart* chart = new QChart();
	chart->legend()->hide();
	chart->addSeries(series);
	chart->createDefaultAxes();
	chart->axes(Qt::Horizontal).first()->hide();
    chart->axes(Qt::Vertical).first()->hide();
	chart->setBackgroundVisible(false);
	chart->setMargins(QMargins(0, 0, 0, 0));
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout* layout = new QVBoxLayout(ui->heartRateChartBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}

void DashboardScreen::setupActiveEnergyChart()
{
    if (!ui->activeEnergyChartBox) return;

    QBarSet* set0 = new QBarSet("Energy");
    *set0 << 300 << 450 << 250 << 600 << 742;
    set0->setColor(QColor("#A0C4FF"));

    QBarSeries* series = new QBarSeries();
    series->append(set0);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));

    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->hide();
    chart->axes(Qt::Vertical).first()->hide();

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");

    QVBoxLayout* layout = new QVBoxLayout(ui->activeEnergyChartBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}
