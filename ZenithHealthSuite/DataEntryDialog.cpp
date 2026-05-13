#include "DataEntryDialog.h"
#include "ui_DataEntryDialog.h"
#include "DataBaseManager.h"
#include<QMessageBox>



DataEntryDialog::DataEntryDialog(QWidget* parent) 
	: QDialog(parent),
	  ui(new Ui::DataEntryDialog)
{
	ui->setupUi(this);
	ui->logDateEdit->setDate(QDate::currentDate());
}

DataEntryDialog::~DataEntryDialog() {
	delete ui;
}

void DataEntryDialog::setPreviousWeight(double lastWeight) {
	ui->weightSpinBox->setValue(lastWeight);
}

void DataEntryDialog::on_buttonBox_accepted() {
	QDate logDate=ui->logDateEdit->date();

	QString exercise = ui->exerciseComboBox->currentText();
	int duration = ui->durationSpinBox->value();
	QString intensity = ui->intensityComboBox->currentText();
	int caloriesBurned = ui->calorieburnedSpinBox->value();

	int steps = ui->stepsSpinBox->value();
	double sleep = ui->sleepSpinBox->value();
	double water = ui->waterintakeSpinBox->value();
	int heartRate = ui->heartrateSpinBox->value();
	double weight = ui->weightSpinBox->value();

	int caloriesEaten = ui->calorieseatenSpinBox->value();
	int protein = ui->proteinSpinBox->value();
	int carbs = ui->carbsSpinBox->value();
	int fats = ui->fatsSpinBox->value();

	DatabaseManager dbManager;
	QString currentUsername = "TestUser";
	bool success = true;

	if (duration > 0) {
		success &= dbManager.addActivityRecord(currentUsername, logDate, exercise, duration, intensity, caloriesBurned);
	}

	success &= dbManager.saveVitalsData(currentUsername, logDate, steps, sleep, water, heartRate, weight);
	success &= dbManager.saveNutritionData(currentUsername, logDate, caloriesEaten, protein, carbs, fats);

	if (success) {
		QMessageBox::information(this, "Success", "Aapka data successfully save ho gaya hai!");
	}
	else {
		QMessageBox::warning(this, "Error", "Data save hone mein masla aaya hai.");
	}
}


	

