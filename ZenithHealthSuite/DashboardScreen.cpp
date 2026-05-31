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

DashboardScreen::DashboardScreen(const user& currentUserObj, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::DashboardScreen)
    , loggedInUser(currentUserObj)
{
    ui->setupUi(this);
    this->showMaximized();

    this->setWindowTitle("ZenithHealthSuite - Dashboard [" + loggedInUser.getName() + "]");

    setupHeartRateChart();
    setupActiveEnergyChart();

    loadRealData();
}

DashboardScreen::~DashboardScreen()
{
    delete ui;
}

void DashboardScreen::on_syncDataButton_clicked()
{
    ui->syncDataButton->setText("Syncing...");
    ui->syncDataButton->setEnabled(false);

    QTimer::singleShot(1500, this, [=]() {
        dbManager.markDataAsSynced(loggedInUser.getUsername());

        loadRealData();

        ui->syncDataButton->setText("Sync Data");
        ui->syncDataButton->setEnabled(true);

        QMessageBox::information(this, "Sync Complete", "All local data successfully synchronized to the cloud!");
        });
}

void DashboardScreen::on_dashboardButton_clicked() {
    qDebug() << "Already on the Dashboard!";
}

void DashboardScreen::on_analyticsButton_clicked() {
    analytics* analyticsScreen = new analytics(loggedInUser);
    analyticsScreen->show();
    this->close();
}

void DashboardScreen::on_activityButton_clicked() {
    Activity* activityScreen = new Activity(loggedInUser);
    activityScreen->show();
    this->close();
}

void DashboardScreen::on_goalsButton_clicked() {
    Goals* goalsscreen = new Goals(loggedInUser);
    goalsscreen->show();
    this->close();
}

void DashboardScreen::on_historyButton_clicked() {
    HistoryScreen* historyScreen = new HistoryScreen(loggedInUser);
    historyScreen->show();
    this->close();
}

void DashboardScreen::on_settingsButton_clicked() { qDebug() << "Settings coming soon..."; }
void DashboardScreen::on_supportButton_clicked() { qDebug() << "Support coming soon..."; }
void DashboardScreen::on_profileButton_clicked() { qDebug() << "User profile..."; }
void DashboardScreen::on_bellButton_clicked() { qDebug() << "No new notifications."; }

void DashboardScreen::updateDashboardMetrics(int caloriesConsumed, int caloriesGoal, int steps, int activeEnergy)
{
    if (caloriesGoal == 0) caloriesGoal = 1;

    int caloriesPercentage = (static_cast<double>(caloriesConsumed) / caloriesGoal) * 100;
    if (caloriesPercentage > 100) caloriesPercentage = 100;

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

    QDate today = QDate::currentDate();
    QMap<QDate, int> chartData;

    for (int i = 6; i >= 0; --i) {
        chartData[today.addDays(-i)] = 0;
    }

    QSqlQuery query;
    query.prepare("SELECT record_date, AVG(heart_rate) FROM VitalsLog "
        "WHERE username = :usr AND record_date >= date('now', '-6 days') "
        "GROUP BY record_date ORDER BY record_date ASC");
    query.bindValue(":usr", loggedInUser.getUsername());

    if (query.exec()) {
        while (query.next()) {
            QDate dbDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
            if (chartData.contains(dbDate)) {
                chartData[dbDate] = query.value(1).toInt();
            }
        }
    }

    int xPoint = 0;
    int lastKnownHR = 70;
    for (auto it = chartData.begin(); it != chartData.end(); ++it) {
        if (it.value() > 0) {
            lastKnownHR = it.value();
        }
        series->append(xPoint, lastKnownHR);
        xPoint++;
    }

    QPen pen(QColor("#D32F2F"));
    pen.setWidth(3);
    series->setPen(pen);

    QChart* chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    if (!chart->axes(Qt::Horizontal).isEmpty()) chart->axes(Qt::Horizontal).first()->hide();
    if (!chart->axes(Qt::Vertical).isEmpty()) chart->axes(Qt::Vertical).first()->hide();
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

    QDate today = QDate::currentDate();
    QMap<QDate, int> energyData;

    for (int i = 6; i >= 0; --i) {
        energyData[today.addDays(-i)] = 0;
    }

    QSqlQuery query;
    query.prepare("SELECT record_date, SUM(calories_burned) FROM ActivityLog "
        "WHERE username = :usr AND record_date >= date('now', '-6 days') "
        "GROUP BY record_date ORDER BY record_date ASC");
    query.bindValue(":usr", loggedInUser.getUsername());

    if (query.exec()) {
        while (query.next()) {
            QDate dbDate = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
            if (energyData.contains(dbDate)) {
                energyData[dbDate] = query.value(1).toInt();
            }
        }
    }

    int xPoint = 0;
    for (auto it = energyData.begin(); it != energyData.end(); ++it) {
        series->append(xPoint, it.value());
        xPoint++;
    }

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
            int maxEnergy = 800;
            for (int val : energyData.values()) { if (val > maxEnergy) maxEnergy = val + 200; }
            axisY->setRange(0, maxEnergy);
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

void DashboardScreen::loadRealData()
{
    QDate currentDate = QDate::currentDate();
    DailyMetrics metrics = dbManager.getDailyMetrics(loggedInUser.getUsername(), currentDate);

    int goalCals = metrics.caloriesGoal > 0 ? metrics.caloriesGoal : (loggedInUser.getTargetWeight() * 24 * 1.2);
    if (goalCals == 0) { goalCals = 2400; }

    updateDashboardMetrics(metrics.caloriesConsumed, goalCals, metrics.stepCount, metrics.activeEnergy);

    QLocale locale(QLocale::English);
    ui->label->setText("/ " + locale.toString(goalCals) + " KCAL");

    QString macrosString = QString("PROTEIN: %1G                 CARBS: %2G                 FATS: %3G")
        .arg(metrics.protien).arg(metrics.carbs).arg(metrics.fats);
    ui->macrosTextLabel->setText(macrosString);

    ui->sleepTimeLabel->setText(QString("%1H %2M").arg(metrics.sleepHours).arg(metrics.sleepMinutes));
    ui->recoveryScoreLabel->setText("OPTIMAL RECOVERY " + QString::number(metrics.recoveryPercentage) + "%");
    if (ui->sleepProgressBar) { ui->sleepProgressBar->setValue(metrics.recoveryPercentage); }
    ui->heartRateValueLabel->setText(QString::number(metrics.heartRate));

    QSqlQuery waterQuery;
    waterQuery.prepare("SELECT SUM(water_l) FROM NutritionLog WHERE username = :usr AND record_date = :date");
    waterQuery.bindValue(":usr", loggedInUser.getUsername());
    waterQuery.bindValue(":date", currentDate.toString("yyyy-MM-dd"));

    double totalWater = 0.0;
    if (waterQuery.exec() && waterQuery.next()) {
        totalWater = waterQuery.value(0).toDouble();
    }

    QString hydroStatus = metrics.hydrationStatus;
    if (hydroStatus.isEmpty()) {
        if (totalWater >= 3.5) hydroStatus = "OPTIMAL";
        else if (totalWater >= 2.0) hydroStatus = "MODERATE (DRINK WATER)";
        else hydroStatus = (totalWater == 0) ? "PENDING" : "LOW (DEHYDRATED)";
    }

    QString recStatus = metrics.recoveryStatus;
    if (recStatus.isEmpty()) {
        if (metrics.sleepHours >= 7) recStatus = "OPTIMAL";
        else if (metrics.sleepHours >= 5) recStatus = "MODERATE";
        else recStatus = (metrics.sleepHours == 0) ? "PENDING" : "LOW";
    }

    QString stressStatus = metrics.stressStatus;
    if (stressStatus.isEmpty()) {
        if (metrics.heartRate == 0) stressStatus = "PENDING";
        else if (metrics.heartRate >= 100) stressStatus = "HIGH";
        else if (metrics.heartRate >= 80) stressStatus = "MODERATE";
        else stressStatus = "OPTIMAL";
    }

    ui->hydrationStatusLabel->setText("HYDRATION: " + hydroStatus.toUpper());
    ui->recoveryStatusLabel->setText("RECOVERY: " + recStatus.toUpper());
    ui->stressStatusLabel->setText("STRESS: " + stressStatus.toUpper());

    if (hydroStatus.contains("MODERATE") || hydroStatus.contains("LOW")) ui->hydrationStatusLabel->setStyleSheet("color: #E65100;");
    else ui->hydrationStatusLabel->setStyleSheet("color: #2E7D32;");

    if (recStatus == "LOW") ui->recoveryStatusLabel->setStyleSheet("color: #D32F2F;");
    else if (recStatus == "OPTIMAL") ui->recoveryStatusLabel->setStyleSheet("color: #2E7D32;");
    else ui->recoveryStatusLabel->setStyleSheet("color: #1558A8;");

    if (stressStatus == "HIGH") ui->stressStatusLabel->setStyleSheet("color: #D32F2F;");
    else if (stressStatus == "MODERATE") ui->stressStatusLabel->setStyleSheet("color: #E65100;");
    else ui->stressStatusLabel->setStyleSheet("color: #2E7D32;");

    QList<ActivityRecord> logs;
    QSqlQuery logQuery;

    logQuery.prepare("SELECT exercise_name, duration_mins, intensity, calories_burned, sync_status "
        "FROM ActivityLog WHERE username = :usr ORDER BY record_date DESC LIMIT 5");
    logQuery.bindValue(":usr", loggedInUser.getUsername());

    if (logQuery.exec()) {
        while (logQuery.next()) {
            ActivityRecord record;
            record.exerciseName = logQuery.value("exercise_name").toString();
            record.durationMins = logQuery.value("duration_mins").toInt();
            record.intensity = logQuery.value("intensity").toString();
            record.caloriesBurned = logQuery.value("calories_burned").toInt();
            record.syncStatus = logQuery.value("sync_status").toString();
            logs.append(record);
        }
    }

    if (ui->activityTable) {
        ui->activityTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->activityTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->activityTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

        ui->activityTable->setStyleSheet(R"(
            QTableWidget { 
                border: none; 
                background-color: transparent; 
            }
            QScrollBar:vertical { 
                border: none; 
                background: #F1F5F9; 
                width: 8px; 
                border-radius: 4px; 
                margin: 0px; 
            }
            QScrollBar::handle:vertical { 
                background: #1558A8; 
                min-height: 20px; 
                border-radius: 4px; 
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { 
                border: none; 
                background: none; 
                height: 0px; 
            }
            QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { 
                background: none; 
            }
        )");
        ui->activityTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->activityTable->verticalHeader()->setDefaultSectionSize(35);
        ui->activityTable->verticalHeader()->setVisible(false);
        ui->activityTable->setRowCount(logs.size());
        for (int i = 0; i < logs.size(); ++i) {
            QTableWidgetItem* exerciseItem = new QTableWidgetItem(logs[i].exerciseName);
            QTableWidgetItem* durationItem = new QTableWidgetItem(QString::number(logs[i].durationMins) + " mins");
            QTableWidgetItem* intensityItem = new QTableWidgetItem(logs[i].intensity);
            QTableWidgetItem* caloriesItem = new QTableWidgetItem(QString::number(logs[i].caloriesBurned) + " kcal");

            exerciseItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            durationItem->setTextAlignment(Qt::AlignCenter);
            intensityItem->setTextAlignment(Qt::AlignCenter);
            caloriesItem->setTextAlignment(Qt::AlignCenter);

            ui->activityTable->setItem(i, 0, exerciseItem);
            ui->activityTable->setItem(i, 1, durationItem);
            ui->activityTable->setItem(i, 2, intensityItem);
            ui->activityTable->setItem(i, 3, caloriesItem);

            QWidget* containerWidget = new QWidget(this);
            QHBoxLayout* layout = new QHBoxLayout(containerWidget);
            layout->setContentsMargins(0, 0, 0, 0);

            QLabel* statusBadge = new QLabel(containerWidget);
            statusBadge->setAlignment(Qt::AlignCenter);
            statusBadge->setText(logs[i].syncStatus);

            if (logs[i].syncStatus == "PENDING") {
                statusBadge->setStyleSheet("background-color: #FFF3E0; color: #E65100; border-radius: 6px; padding: 4px 12px; font-weight: bold; font-size: 11px;");
            }
            else {
                statusBadge->setStyleSheet("background-color: #E8F5E9; color: #2E7D32; border-radius: 6px; padding: 4px 12px; font-weight: bold; font-size: 11px;");
            }

            layout->addWidget(statusBadge);
            layout->setAlignment(Qt::AlignCenter);
            ui->activityTable->setCellWidget(i, 4, containerWidget);
        }
    }
}