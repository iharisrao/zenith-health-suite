#include "DashboardScreen.h"
#include "ui_DashboardScreen.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QBarSeries>
#include <QBarSet>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QVBoxLayout>
#include <QDate>
#include <QHBoxLayout>
#include <QStackedBarSeries>
#include <QSplineSeries>


DashboardScreen::DashboardScreen(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardScreen)
{
    ui->setupUi(this);
    this->showMaximized();

	setupHeartRateChart();
    setupActiveEnergyChart();
    setupTableBadges();
   
	updateDashboardMetrics(1840, 2400, 9248, 742);
}

DashboardScreen::~DashboardScreen()
{
    delete ui;
}


void DashboardScreen::on_dashboardButton_clicked() {
    qDebug() << "Already on the Dashboard!";
}

void DashboardScreen::on_analyticsButton_clicked() {
    qDebug() << "Analytics coming soon...";
}

void DashboardScreen::on_activityButton_clicked() {
    Activity* activityScreen = new Activity();
    activityScreen->show();
    this->close();
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
    chartView->setMinimumSize(0, 0);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout(ui->heartRateChartBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}


void DashboardScreen::setupActiveEnergyChart()
{
    if (!ui->activeEnergyChartBox) return;

    QSplineSeries* series = new QSplineSeries();
    series->append(0, 300);
    series->append(1, 450);
    series->append(2, 250);
    series->append(3, 600);
    series->append(4, 742);

    QPen pen(QColor("#1976D2")); 
    pen.setWidth(4);
    series->setPen(pen);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();

    if (!chart->axes(Qt::Horizontal).isEmpty()) chart->axes(Qt::Horizontal).first()->hide();
    if (!chart->axes(Qt::Vertical).isEmpty()) {
        QValueAxis* axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        if (axisY) {
            axisY->setRange(200, 800); 
            axisY->hide();
        }
    }
    chart->setBackgroundVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setMinimumSize(0, 0);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* layout = new QVBoxLayout(ui->activeEnergyChartBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chartView);
}

void DashboardScreen::setupTableBadges()
{
    if (!ui->activityTable) return;

    for (int i = 0; i < ui->activityTable->rowCount(); ++i) {

        
        QWidget* containerWidget = new QWidget(this);
        QHBoxLayout* layout = new QHBoxLayout(containerWidget);
        layout->setContentsMargins(0, 0, 0, 0); 

        
        QLabel* statusBadge = new QLabel(containerWidget);
        statusBadge->setAlignment(Qt::AlignCenter);

        if (i == 2) { 
            statusBadge->setText("PENDING");
            statusBadge->setStyleSheet(
                "background-color: #FFF3E0; color: #E65100; "
                "border-radius: 6px; padding: 4px 12px; font-weight: bold; font-size: 11px;"
            );
        }
        else { 
            statusBadge->setText("SYNCED");
            statusBadge->setStyleSheet(
                "background-color: #E8F5E9; color: #2E7D32; "
                "border-radius: 6px; padding: 4px 12px; font-weight: bold; font-size: 11px;"
            );
        }

        
        layout->addWidget(statusBadge);
        layout->setAlignment(Qt::AlignCenter);

        
        ui->activityTable->setCellWidget(i, 4, containerWidget);
    }
}
   

