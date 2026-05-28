#include "analytics.h"
#include "ui_analytics.h"
#include <QVBoxLayout>
#include <QSplineSeries>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QDate>
#include <QSqlQuery> 
#include <QDebug>

analytics::analytics(const user& currentUserObj, QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::analyticsClass),
    loggedInUser(currentUserObj),
    currentDays(7)
{
    ui->setupUi(this);
    this->showMaximized();

    this->setWindowTitle("ZenithHealthSuite - Analytics [" + loggedInUser.getName() + "]");

    connect(ui->btn7Days, &QPushButton::clicked, this, &analytics::on_btn7Days_clicked);
    connect(ui->btn30Days, &QPushButton::clicked, this, &analytics::on_btn30Days_clicked);
    updateButtonStyles();

    loadRealData();
}

analytics::~analytics()
{
    delete ui;
}

void analytics::on_syncDataButton_clicked()
{
    ui->syncDataButton->setText("Syncing...");
    ui->syncDataButton->setEnabled(false);

    QTimer::singleShot(1500, this, [=]() {
        loadRealData();

        ui->syncDataButton->setText("Sync Data");
        ui->syncDataButton->setEnabled(true);

        QMessageBox::information(this, "Sync Complete", "All local data successfully synchronized!");
        });
}

void analytics::on_btn7Days_clicked() {
    currentDays = 7;
    updateButtonStyles();
    loadRealData();
}

void analytics::on_btn30Days_clicked() {
    currentDays = 30;
    updateButtonStyles();
    loadRealData();
}

void analytics::updateButtonStyles() {
    QString activeStyle = "background-color: #1558A8; color: white; border-radius: 15px;";
    QString inactiveStyle = "background-color: transparent; color: #64748B; border-radius: 15px; border: 1px solid #E2E8F0;";

    ui->btn7Days->setStyleSheet(currentDays == 7 ? activeStyle : inactiveStyle);
    ui->btn30Days->setStyleSheet(currentDays == 30 ? activeStyle : inactiveStyle);
}

void analytics::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen(loggedInUser);
    dashboard->show();
    this->close();
}
void analytics::on_analyticsButton_clicked() { qDebug() << "Already on the Analytics!"; }
void analytics::on_activityButton_clicked() {
    Activity* activityScreen = new Activity(loggedInUser);
    activityScreen->show();
    this->close();
}
void analytics::on_goalsButton_clicked() {
    Goals* goalsscreen = new Goals(loggedInUser);
    goalsscreen->show();
    this->close();
}
void analytics::on_historyButton_clicked() {
    HistoryScreen* historyScreen = new HistoryScreen(loggedInUser);
    historyScreen->show();
    this->close();
}
void analytics::on_settingsButton_clicked() { qDebug() << "Settings coming soon..."; }
void analytics::on_supportButton_clicked() { qDebug() << "Support coming soon..."; }
void analytics::on_profileButton_clicked() { qDebug() << "User profile..."; }
void analytics::on_bellButton_clicked() { qDebug() << "No new notifications."; }

void analytics::loadRealData()
{
    QDate endDate = QDate::currentDate();
    QDate startDate = endDate.addDays(-currentDays + 1);

    QString startStr = startDate.toString("yyyy-MM-dd");
    QString endStr = endDate.toString("yyyy-MM-dd");
    QString usr = loggedInUser.getUsername();

    QSqlQuery q;

    int activeDays = 1; 
    q.prepare("SELECT COUNT(DISTINCT record_date) FROM NutritionLog "
        "WHERE username = :usr AND record_date BETWEEN :start AND :end");
    q.bindValue(":usr", usr);
    q.bindValue(":start", startStr);
    q.bindValue(":end", endStr);

    if (q.exec() && q.next() && q.value(0).toInt() > 0) {
        activeDays = q.value(0).toInt();
    }

    q.prepare("SELECT SUM(calories), SUM(protein_g), SUM(carbs_g), SUM(fats_g), SUM(water_l) "
        "FROM NutritionLog WHERE username = :usr AND record_date BETWEEN :start AND :end");
    q.bindValue(":usr", usr);
    q.bindValue(":start", startStr);
    q.bindValue(":end", endStr);

    DailyMetrics avgMetrics = { 0 };
    double avgWater = 0.0;

    if (q.exec() && q.next()) {
        avgMetrics.caloriesConsumed = q.value(0).toInt() / activeDays;
        avgMetrics.protien = q.value(1).toInt() / activeDays;
        avgMetrics.carbs = q.value(2).toInt() / activeDays;
        avgMetrics.fats = q.value(3).toInt() / activeDays;
        avgWater = q.value(4).toDouble() / activeDays;
    }

    populateTable(avgMetrics);

    ui->lblHydrationValue->setText(QString::number(avgWater, 'f', 1) + "L");
    double targetWater = loggedInUser.getWeight() * 0.035;
    if (targetWater <= 0) targetWater = 2.5;
    ui->lblHydrationSub->setText(QString("Daily Target: %1L").arg(targetWater, 0, 'f', 1));

    q.prepare("SELECT AVG(sleep_hours), AVG(heart_rate), AVG(bp_sys), AVG(bp_dia) FROM VitalsLog "
        "WHERE username = :usr AND record_date BETWEEN :start AND :end");
    q.bindValue(":usr", usr);
    q.bindValue(":start", startStr);
    q.bindValue(":end", endStr);

    double avgSleep = 0;
    int avgHR = 0, avgSys = 0, avgDia = 0;

    if (q.exec() && q.next()) {
        avgSleep = q.value(0).toDouble();
        avgHR = q.value(1).toInt();
        avgSys = q.value(2).toInt();
        avgDia = q.value(3).toInt();
    }

    int sleepPercentage = (avgSleep > 0) ? (avgSleep * 100) / 8.0 : 0;
    if (sleepPercentage > 100) sleepPercentage = 100;
    setupSleepChart(sleepPercentage);

    if (avgSys > 0 && avgDia > 0) {
        ui->lblBloodPressureValue->setText(QString("%1/%2").arg(avgSys).arg(avgDia));
        if (avgSys <= 120 && avgDia <= 80) ui->lblBloodPressureSub->setText("Normal Avg");
        else ui->lblBloodPressureSub->setText("Elevated Avg");
    }
    else {
        ui->lblBloodPressureValue->setText("0/0");
        ui->lblBloodPressureSub->setText("No Data");
    }

    setupHeartRateChart(avgHR);

    if (avgHR == 0) {
        ui->lblStressValue->setText("Pending");
        ui->lblStressSub->setText("No HR Data");
    }
    else if (avgHR > 90) {
        ui->lblStressValue->setText("High");
        ui->lblStressSub->setText("Elevated HR");
    }
    else if (avgHR >= 70) {
        ui->lblStressValue->setText("Moderate");
        ui->lblStressSub->setText("Slightly Elevated");
    }
    else {
        ui->lblStressValue->setText("Low");
        ui->lblStressSub->setText("Stable");
    }

    QList<int> historicalCalories;
    QStringList daysList;

    for (int i = currentDays - 1; i >= 0; --i) {
        QDate d = QDate::currentDate().addDays(-i);

        if (currentDays == 30) {
            if (i % 5 == 0 || i == 0) {
                daysList << d.toString("dd"); 
            }
            else {
                daysList << ""; 
            }
        }
        else {
            daysList << d.toString("ddd");
        }

        QSqlQuery barQ;
        barQ.prepare("SELECT SUM(calories_burned) FROM ActivityLog WHERE username = :usr AND record_date = :date");
        barQ.bindValue(":usr", loggedInUser.getUsername());
        barQ.bindValue(":date", d.toString("yyyy-MM-dd"));

        if (barQ.exec() && barQ.next()) {
            historicalCalories << barQ.value(0).toInt();
        }
        else {
            historicalCalories << 0;
        }
    }
    setupBarChart(historicalCalories, daysList);

    double bmr = loggedInUser.getWeight() * 24.0;
    if (bmr <= 0) bmr = 1500;

    QString bmrText = (bmr >= 1000) ? QString::number(bmr / 1000.0, 'f', 1) + "k" : QString::number(bmr, 'f', 0);
    ui->lblMetabolicValue->setText(bmrText);
    ui->lblMetabolicSub->setText("BMR (kcal/day)");
}

void analytics::setupHeartRateChart(int baseHR)
{
    if (ui->frameHeartRate->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = ui->frameHeartRate->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ui->frameHeartRate->layout();
    }

    QSplineSeries* series = new QSplineSeries();

    if (baseHR == 0) {
        series->append(0, 0); series->append(24, 0);
    }
    else {
        series->append(0, baseHR - 10);
        series->append(6, baseHR + 5);
        series->append(12, baseHR - 2);
        series->append(18, baseHR + 8);
        series->append(24, baseHR - 5);
    }

    QPen pen(QColor("#1558A8"));
    pen.setWidth(3);
    series->setPen(pen);

    QChart* chart = new QChart();
    chart->addSeries(series);

    QValueAxis* axisX = new QValueAxis();
    axisX->setGridLineVisible(false);
    axisX->setLineVisible(false);
    axisX->setRange(0, 24);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    axisY->setVisible(false);
    axisY->setRange(30, 150);
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

    new QVBoxLayout(ui->frameHeartRate);
    ui->frameHeartRate->layout()->setContentsMargins(0, 5, 0, 0);
    ui->frameHeartRate->layout()->addWidget(chartView);
}

void analytics::setupSleepChart(int sleepPercentage)
{
    if (ui->frameSleep->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = ui->frameSleep->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ui->frameSleep->layout();
    }

    QPieSeries* series = new QPieSeries();
    series->setHoleSize(0.65);

    QPieSlice* slice1 = series->append("Optimal", sleepPercentage);
    slice1->setBrush(QColor("#1558A8"));
    slice1->setBorderColor(Qt::transparent);

    QPieSlice* slice2 = series->append("Other", 100 - sleepPercentage);
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

    QLabel* percentageLabel = new QLabel(QString::number(sleepPercentage) + "%");
    percentageLabel->setAlignment(Qt::AlignCenter);
    percentageLabel->setStyleSheet("color: #0F172A; font-size: 26px; font-weight: 900; background: transparent;");

    QGridLayout* layout = new QGridLayout(ui->frameSleep);
    layout->setContentsMargins(0, 10, 0, 0);
    layout->addWidget(chartView, 0, 0);
    layout->addWidget(percentageLabel, 0, 0, Qt::AlignCenter);
}

void analytics::setupBarChart(QList<int> weeklyData, QStringList daysList)
{
    if (ui->frameBarChart->layout() != nullptr) {
        QLayoutItem* item;
        while ((item = ui->frameBarChart->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete ui->frameBarChart->layout();
    }

    QBarSet* set0 = new QBarSet("Intensity");

    for (int val : weeklyData) {
        *set0 << val;
    }
    set0->setColor(QColor("#1558A8"));

    QBarSeries* series = new QBarSeries();
    series->append(set0);

    if (weeklyData.size() > 7) {
        series->setBarWidth(0.9); 
    }
    else {
        series->setBarWidth(0.6);
    }

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundVisible(false);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(daysList);
    axisX->setGridLineVisible(false);
    axisX->setLineVisible(false);

    QFont labelsFont;
    labelsFont.setPixelSize(10); 
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

    new QVBoxLayout(ui->frameBarChart);
    ui->frameBarChart->layout()->setContentsMargins(0, 0, 0, 0);
    ui->frameBarChart->layout()->addWidget(chartView);
}

void analytics::populateTable(const DailyMetrics& metrics)
{
    ui->TableWidget->setColumnCount(4);
    ui->TableWidget->setRowCount(4);
    ui->TableWidget->setFrameShape(QFrame::NoFrame);
    ui->TableWidget->setShowGrid(false);

    ui->TableWidget->horizontalHeader()->setVisible(true);
    ui->TableWidget->setHorizontalHeaderLabels(QStringList() << "METRIC" << "DAILY AVERAGE" << "DAILY TARGET" << "STATUS");

    ui->TableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->TableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TableWidget->verticalHeader()->setVisible(false);
    ui->TableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->TableWidget->verticalHeader()->setDefaultSectionSize(28);
    ui->TableWidget->setFocusPolicy(Qt::NoFocus);
    ui->TableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->TableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->TableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    double tWeight = loggedInUser.getTargetWeight();
    if (tWeight <= 0) tWeight = 70.0;

    int targetCals = tWeight * 24 * 1.2;
    int targetProtein = tWeight * 2.2;
    int targetFats = tWeight * 1.0;
    int targetCarbs = (targetCals - (targetProtein * 4) - (targetFats * 9)) / 4;

    QString calStatus = "NEEDS WORK";
    if (metrics.caloriesConsumed >= targetCals * 0.9 && metrics.caloriesConsumed <= targetCals * 1.1) calStatus = "OPTIMAL";
    else if (metrics.caloriesConsumed > targetCals * 1.1) calStatus = "HIGH";

    QString proStatus = "LOW";
    if (metrics.protien >= targetProtein * 0.9) proStatus = "OPTIMAL";

    QString carbStatus = "LOW";
    if (metrics.carbs == 0) carbStatus = "LOW";
    else if (metrics.carbs > targetCarbs * 1.1) carbStatus = "HIGH";
    else carbStatus = "OPTIMAL";

    QString fatStatus = "LOW";
    if (metrics.fats == 0) fatStatus = "LOW";
    else if (metrics.fats > targetFats * 1.1) fatStatus = "HIGH";
    else fatStatus = "OPTIMAL";

    auto createItem = [](QString text) {
        QTableWidgetItem* item = new QTableWidgetItem(text);
        item->setTextAlignment(Qt::AlignCenter);
        return item;
        };

    ui->TableWidget->setItem(0, 0, createItem("Total Calories"));
    ui->TableWidget->setItem(0, 1, createItem(QString::number(metrics.caloriesConsumed) + " kcal"));
    ui->TableWidget->setItem(0, 2, createItem(QString::number(targetCals) + " kcal"));
    ui->TableWidget->setItem(0, 3, createItem(calStatus));

    ui->TableWidget->setItem(1, 0, createItem("Protein Intake"));
    ui->TableWidget->setItem(1, 1, createItem(QString::number(metrics.protien) + " g"));
    ui->TableWidget->setItem(1, 2, createItem(QString::number(targetProtein) + " g"));
    ui->TableWidget->setItem(1, 3, createItem(proStatus));

    ui->TableWidget->setItem(2, 0, createItem("Carbohydrates"));
    ui->TableWidget->setItem(2, 1, createItem(QString::number(metrics.carbs) + " g"));
    ui->TableWidget->setItem(2, 2, createItem(QString::number(targetCarbs) + " g"));
    ui->TableWidget->setItem(2, 3, createItem(carbStatus));

    ui->TableWidget->setItem(3, 0, createItem("Dietary Fats"));
    ui->TableWidget->setItem(3, 1, createItem(QString::number(metrics.fats) + " g"));
    ui->TableWidget->setItem(3, 2, createItem(QString::number(targetFats) + " g"));
    ui->TableWidget->setItem(3, 3, createItem(fatStatus));
}