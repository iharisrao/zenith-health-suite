#include "Activity.h"
#include "ui_Activity.h"
#include <QMessageBox>
#include <QDate>         
#include <QDebug>

Activity::Activity(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::Activity)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("ZenithHealthSuite - Activity");

    setupCurrentDate();
    ui->tabWidget->setCurrentIndex(0);
    loadIntegratedMetrics();
}

Activity::~Activity()
{
    delete ui;
}

void Activity::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen();
    dashboard->show();
    this->close();
}


void Activity::on_analyticsButton_clicked() {
    qDebug() << "Analytics coming soon...";
}

void Activity::on_activityButton_clicked() {
    qDebug() << "Already on the Activity!";
}

void Activity::on_goalsButton_clicked() {
    qDebug() << "Goals coming soon...";
}

void Activity::on_historyButton_clicked() {
    HistoryScreen* historyScreen = new HistoryScreen();
    historyScreen->show();
    this->close();
}

void Activity::on_settingsButton_clicked() {
    qDebug() << "Settings coming soon...";
}

void Activity::on_supportButton_clicked() {
    qDebug() << "Support coming soon...";
}

void Activity::on_profileButton_clicked() {
    qDebug() << "User profile...";
}

void Activity::on_bellButton_clicked() {
    qDebug() << "No new notifications.";
}


void Activity::setupCurrentDate()
{
    QString todayDate = QDate::currentDate().toString("dd MMM, yyyy");
    ui->btnHeaderDate->setText(todayDate);
}

void Activity::loadIntegratedMetrics()
{
    qDebug() << "Metrics database se load hone ke liye tayar hain...";
}

void Activity::on_btnSaveActivity_clicked()
{
    QString exerciseName = ui->comboExerciseName_2->currentText();
    int duration = ui->spinDuration->value();
    int intensity = ui->spinIntensity->value();
    QString caloriesBurned = ui->lineCalories->text();

    if (caloriesBurned.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter Calories Burned.");
        return;
    }

    qDebug() << "Activity Data Ready:" << exerciseName << duration << intensity << caloriesBurned;
    QMessageBox::information(this, "Success", "Activity Data extracted successfully! Ready for DB.");
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
    int sleepHours = ui->sleepSpinBox->value();

    qDebug() << "Vitals Data Ready:" << weight << heartRate << bpSys << "/" << bpDia;
    QMessageBox::information(this, "Success", "Vitals Data extracted successfully! Ready for DB.");
}

void Activity::on_btnSaveMeal_clicked()
{
    QString mealType = ui->comboMealType->currentText();
    QString foodName = ui->lineFoodName->text();
    int calories = ui->spinCals->value();
    int protein = ui->spinProtein->value();
    int carbs = ui->spinCarbs->value();
    int fats = ui->spinFats->value();
    int waterIntake = ui->spinWater->value();
    int caffeine = ui->spinCaffeine->value();

    if (foodName.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter the Food Name.");
        return;
    }

    qDebug() << "Nutrition Data Ready:" << mealType << foodName << calories;
    QMessageBox::information(this, "Success", "Nutrition Data extracted successfully! Ready for DB.");
}