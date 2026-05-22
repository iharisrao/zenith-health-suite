#include "analytics.h"
#include "ui_analytics.h"
#include <QVBoxLayout>
#include <QSplineSeries>
#include <QValueAxis>
#include <QBarCategoryAxis> 

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

void analytics::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen();
    dashboard->show();
    this->close();
}


void analytics::on_analyticsButton_clicked() {
    qDebug() << "Already on the Analytics!";
}

void analytics::on_activityButton_clicked() {
    Activity* activityScreen = new Activity();
    activityScreen->show();
    this->close();
}

void analytics::on_goalsButton_clicked() {
    qDebug() << "Goals coming soon...";
}

void analytics::on_historyButton_clicked() {
	HistoryScreen* historyScreen = new HistoryScreen();
	historyScreen->show();
	this->close();
}

void analytics::on_settingsButton_clicked() {
    qDebug() << "Settings coming soon...";
}

void analytics::on_supportButton_clicked() {
    qDebug() << "Support coming soon...";
}

void analytics::on_profileButton_clicked() {
    qDebug() << "User profile...";
}

void analytics::on_bellButton_clicked() {
    qDebug() << "No new notifications.";
}




void analytics::setupHeartRateChart()
{
    QSplineSeries* series = new QSplineSeries();
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

    QValueAxis* axisX = new QValueAxis();
    axisX->setGridLineVisible(false);
    axisX->setLineVisible(false);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setVisible(false);
    axisY->setRange(30, 90);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setMinimumHeight(60);
    chartView->setMaximumHeight(160); 

    if (!ui->frameHeartRate->layout()) {
        new QVBoxLayout(ui->frameHeartRate);
    }
    ui->frameHeartRate->layout()->setContentsMargins(0, 5, 0, 0);
    ui->frameHeartRate->layout()->addWidget(chartView);
}

void analytics::setupSleepChart()
{
    QPieSeries* series = new QPieSeries();
    series->setHoleSize(0.65);

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

    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setMinimumHeight(80);
    chartView->setMaximumHeight(160); 

    QLabel* percentageLabel = new QLabel("85%");
    percentageLabel->setAlignment(Qt::AlignCenter);
    percentageLabel->setStyleSheet("color: #0F172A; font-size: 26px; font-weight: 900; background: transparent;");

    if (!ui->frameSleep->layout()) {
        new QGridLayout(ui->frameSleep);
    }
    QGridLayout* layout = qobject_cast<QGridLayout*>(ui->frameSleep->layout());
    if (layout) {
        layout->setContentsMargins(0, 10, 0, 0);
        layout->addWidget(chartView, 0, 0);
        layout->addWidget(percentageLabel, 0, 0, Qt::AlignCenter);
    }
}

void analytics::setupBarChart()
{
    QBarSet* set0 = new QBarSet("Intensity");
    *set0 << 2 << 4 << 3 << 7 << 5 << 6 << 2;
    set0->setColor(QColor("#1558A8"));

    QBarSeries* series = new QBarSeries();
    series->append(set0);

    series->setBarWidth(0.6);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QStringList categories;
    categories << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setGridLineVisible(false);
    axisX->setLineVisible(false); 

    QFont labelsFont;
    labelsFont.setPixelSize(11);
    labelsFont.setBold(true);
    axisX->setLabelsFont(labelsFont);
    axisX->setLabelsColor(QColor("#8C98A4"));

    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setVisible(false);
    axisY->setLabelsVisible(false);
    axisY->setLineVisible(false); 
    axisY->setGridLineVisible(false);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    chartView->setStyleSheet("background: transparent; border: none;");
    chartView->setMinimumHeight(60);
    chartView->setMaximumHeight(140); 

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

    ui->TableWidget->setFrameShape(QFrame::NoFrame); 
    ui->TableWidget->setShowGrid(false); 
    ui->TableWidget->verticalHeader()->setVisible(false); 

    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->TableWidget->verticalHeader()->setDefaultSectionSize(28);

    ui->TableWidget->setFocusPolicy(Qt::NoFocus);
    ui->TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->TableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->TableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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