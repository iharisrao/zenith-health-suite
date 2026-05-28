#include "Activity.h"
#include "ui_Activity.h"
#include <QMessageBox>
#include <QDate>         
#include <QDebug>
#include <QTimer>
#include <QSqlQuery>

Activity::Activity(const user& currentUserObj, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Activity),
    loggedInUser(currentUserObj)
{
    ui->setupUi(this);
    this->showMaximized();

    this->setWindowTitle("ZenithHealthSuite - Activity [" + loggedInUser.getName() + "]");

    setupCurrentDate();
    ui->tabWidget->setCurrentIndex(0);

    loadIntegratedMetrics();
}

Activity::~Activity()
{
    delete ui;
}


void Activity::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen(loggedInUser);
    dashboard->show();
    this->close();
}
void Activity::on_analyticsButton_clicked() {
    analytics* analyticsScreen = new analytics(loggedInUser);
    analyticsScreen->show();
    this->close();
}
void Activity::on_activityButton_clicked() { qDebug() << "Already on the Activity!"; }
void Activity::on_goalsButton_clicked() {
    Goals* goalsscreen = new Goals(loggedInUser);
    goalsscreen->show();
    this->close();
}
void Activity::on_historyButton_clicked() {
    HistoryScreen* historyScreen = new HistoryScreen(loggedInUser);
    historyScreen->show();
    this->close();
}
void Activity::on_settingsButton_clicked() { qDebug() << "Settings coming soon..."; }
void Activity::on_supportButton_clicked() { qDebug() << "Support coming soon..."; }
void Activity::on_profileButton_clicked() { qDebug() << "User profile..."; }
void Activity::on_bellButton_clicked() { qDebug() << "No new notifications."; }

void Activity::setupCurrentDate()
{
    QString todayDate = QDate::currentDate().toString("dd MMM, yyyy");
    ui->btnHeaderDate->setText(todayDate);
}


void Activity::on_syncDataButton_clicked()
{

    ui->syncDataButton->setText("Syncing...");
    ui->syncDataButton->setEnabled(false);

    QTimer::singleShot(1500, this, [=]() {
        loadIntegratedMetrics();

        ui->syncDataButton->setText("Sync Data");
        ui->syncDataButton->setEnabled(true);

        QMessageBox::information(this, "Sync Complete", "All local data successfully synchronized!");
        });
}

void Activity::loadIntegratedMetrics()
{
    QDate currentDate = QDate::currentDate();
    DailyMetrics metrics = dbManager.getDailyMetrics(loggedInUser.getUsername(), currentDate);

    ui->lblStepCountValue->setText(QString::number(metrics.stepCount));

    double sleepScore = metrics.sleepHours + (metrics.sleepMinutes / 60.0);
    ui->lblSleepQualityValue->setText(QString::number(sleepScore, 'f', 1));

    QSqlQuery query;
    query.prepare("SELECT SUM(water_l) FROM NutritionLog WHERE username = :usr AND record_date = :date");
    query.bindValue(":usr", loggedInUser.getUsername());
    query.bindValue(":date", currentDate.toString("yyyy-MM-dd"));
    double totalWater = 0;
    if (query.exec() && query.next()) {
        totalWater = query.value(0).toDouble();
    }
    ui->lblWaterIntakeValue->setText(QString::number(totalWater, 'f', 1) + " L");

    if (metrics.caloriesConsumed == 0) {
        ui->lblMacrosValue->setText("Pending");
        ui->lblMacrosValue->setStyleSheet("color: #E65100;"); 
    }
    else {
        ui->lblMacrosValue->setText("Tracking");
        ui->lblMacrosValue->setStyleSheet("color: #2E7D32;"); 
    }
}

void Activity::on_btnSaveActivity_clicked()
{
    QString exerciseName = ui->comboExerciseName_2->currentText();
    int duration = ui->spinDuration->value();
    int intensity = ui->spinIntensity->value();
    QString caloriesBurned = ui->lineCalories->text();

    int steps = ui->spinSteps->value();

    if (caloriesBurned.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter Calories Burned.");
        return;
    }

    if (dbManager.saveActivityRecord(loggedInUser.getUsername(), QDate::currentDate(), exerciseName, duration, intensity, caloriesBurned.toInt(), steps)) {
        QMessageBox::information(this, "Done", "Activity saved successfully!");
        ui->spinDuration->setValue(0);
        ui->spinIntensity->setValue(0);
        ui->lineCalories->clear();
        ui->spinSteps->setValue(0); 

        loadIntegratedMetrics(); 
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save activity.");
    }
}

void Activity::on_btnSaveVitals_clicked()
{
    double weight = ui->spinWeight->value();
    double bodyTemp = ui->spinTemp->value();
    int heartRate = ui->spinHeartRate->value();
    int bpSys = ui->pinBPSys->value();
    int bpDia = ui->spinBPDia->value();
    int bloodSugar = ui->spinSugar->value();
    int stressLevel = ui->spinStress->value();

    double sleepHours = ui->sleepDoubleSpinBox->value();

    if (dbManager.saveVitalsRecord(loggedInUser.getUsername(), QDate::currentDate(), weight, bodyTemp, heartRate, bpSys, bpDia, bloodSugar, stressLevel, sleepHours)) {
        QMessageBox::information(this, "Done", "Vitals saved successfully!");

        loadIntegratedMetrics();
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save vitals.");
    }
}

void Activity::on_btnSaveMeal_clicked()
{
    QString mealType = ui->comboMealType->currentText();
    QString foodName = ui->lineFoodName->text();
    int calories = ui->spinCals->value();
    int protein = ui->spinProtein->value();
    int carbs = ui->spinCarbs->value();
    int fats = ui->spinFats->value();
    double waterIntake = ui->spinWater->value();
    int caffeine = ui->spinCaffeine->value();

    if (foodName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter Food Name.");
        return;
    }

    if (dbManager.saveNutritionRecord(loggedInUser.getUsername(), QDate::currentDate(), mealType, foodName, calories, protein, carbs, fats, waterIntake, caffeine)) {
        QMessageBox::information(this, "Done", "Meal saved successfully!");
        ui->lineFoodName->clear();
        ui->spinCals->setValue(0);
        ui->spinProtein->setValue(0);
        ui->spinCarbs->setValue(0);
        ui->spinFats->setValue(0);
        ui->spinWater->setValue(0);
        ui->spinCaffeine->setValue(0);

        loadIntegratedMetrics(); 
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to save meal.");
    }
}