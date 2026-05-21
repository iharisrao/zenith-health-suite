#include "analytics.h"
#include "ui_analytics.h"
#include <QVBoxLayout>

analytics::analytics(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::analyticsClass)
{
    ui->setupUi(this);
    this->showMaximized();

    setupHeartRateChart();
    setupSleepChart();
    setupBarChart();
    populateTable();
}

analytics::~analytics()
{
    delete ui;
}

void analytics::setupHeartRateChart()
{
    QLineSeries* series = new QLineSeries();
    series->append(0, 60);
    series->append(6, 50);
    series->append(12, 75);
    series->append(18, 55);
    series->append(24, 70);

    QPen pen(QColor("#1558A8"));
    pen.setWidth(3);
    series->setPen(pen);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setMinimumHeight(100);

    if (!ui->frameHeartRate->layout()) {
        new QVBoxLayout(ui->frameHeartRate);
    }
    ui->frameHeartRate->layout()->setContentsMargins(0, 35, 0, 0);
    ui->frameHeartRate->layout()->addWidget(chartView);
}

void analytics::setupSleepChart()
{
    QPieSeries* series = new QPieSeries();
    series->setHoleSize(0.5);

    QPieSlice* slice1 = series->append("Optimal", 85);
    slice1->setBrush(QColor("#1558A8"));
    slice1->setBorderColor(Qt::transparent);

    QPieSlice* slice2 = series->append("Other", 15);
    slice2->setBrush(QColor("#E2E8F0"));
    slice2->setBorderColor(Qt::transparent);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setMinimumHeight(80);

    if (!ui->frameSleep->layout()) {
        new QVBoxLayout(ui->frameSleep);
    }

    ui->frameSleep->layout()->setContentsMargins(0, 25, 0, 0);
    ui->frameSleep->layout()->addWidget(chartView);
}

void analytics::setupBarChart()
{
    QBarSet* set0 = new QBarSet("Intensity");
    *set0 << 2 << 4 << 3 << 7 << 5 << 6 << 2;
    set0->setColor(QColor("#1558A8"));

    QBarSeries* series = new QBarSeries();
    series->append(set0);
    series->setBarWidth(0.5);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QStringList categories;
    categories << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartView->setMinimumHeight(60);

    if (!ui->frameBarChart->layout()) {
        new QVBoxLayout(ui->frameBarChart);
    }
    ui->frameBarChart->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameBarChart->layout()->addWidget(chartView);
}

void analytics::populateTable()
{
    ui->TableWidget->setColumnCount(4);
    ui->TableWidget->setRowCount(4);

    ui->TableWidget->setItem(0, 0, new QTableWidgetItem("Total Calories"));
    ui->TableWidget->setItem(0, 1, new QTableWidgetItem("2450 kcal"));
    ui->TableWidget->setItem(0, 2, new QTableWidgetItem("2500 kcal"));
    ui->TableWidget->setItem(0, 3, new QTableWidgetItem("OPTIMAL"));

    ui->TableWidget->setItem(1, 0, new QTableWidgetItem("Protein Intake"));
    ui->TableWidget->setItem(1, 1, new QTableWidgetItem("140 g"));
    ui->TableWidget->setItem(1, 2, new QTableWidgetItem("150 g"));
    ui->TableWidget->setItem(1, 3, new QTableWidgetItem("GOOD"));

    ui->TableWidget->setItem(2, 0, new QTableWidgetItem("Carbohydrates"));
    ui->TableWidget->setItem(2, 1, new QTableWidgetItem("280 g"));
    ui->TableWidget->setItem(2, 2, new QTableWidgetItem("300 g"));
    ui->TableWidget->setItem(2, 3, new QTableWidgetItem("OPTIMAL"));

    ui->TableWidget->setItem(3, 0, new QTableWidgetItem("Dietary Fats"));
    ui->TableWidget->setItem(3, 1, new QTableWidgetItem("65 g"));
    ui->TableWidget->setItem(3, 2, new QTableWidgetItem("70 g"));
    ui->TableWidget->setItem(3, 3, new QTableWidgetItem("OPTIMAL"));
}