#include "DataEntryDialog.h"
#include "ui_DataEntryDialog.h"

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
	QString logDate = ui->logDateEdit->date().toString("yyyy-MM-dd");

	QString exercise = ui->exerciseComboBox->currentText();
	int duration = ui->durationSpinBox->value();
	QString intensity = ui->intensityComboBox->currentText();
	int caloriesBurned = ui->calorieburnedSpinBox->value();

	int steps = ui->stepsSpinBox->value();
	double sleep = ui->sleepSpinBox->value();
	double water = ui->waterintakeSpinBox->value();
	double heartRate = ui->heartrateSpinBox->value();
	double weight = ui->weightSpinBox->value();

	int caloriesEaten = ui->calorieseatenSpinBox->value();
	int protein = ui->proteinSpinBox->value();
	int carbs = ui->carbsSpinBox->value();
	int fats = ui->fatsSpinBox->value();


	qDebug() << "========== NEW HEALTH LOG RECORDED ==========";
	qDebug() << "Date:" << logDate;
	qDebug() << "--- ACTIVITY ---";
	qDebug() << "Exercise:" << exercise << "(" << intensity << ")";
	qDebug() << "Duration:" << duration << "mins | Burned:" << caloriesBurned << "kcal";
	qDebug() << "--- VITALS ---";
	qDebug() << "Steps:" << steps << "| Sleep:" << sleep << "hrs | Water:" << water << "L";
	qDebug() << "Heart Rate:" << heartRate << "bpm | Weight:" << weight << "kg";
	qDebug() << "--- NUTRITION ---";
	qDebug() << "Intake:" << caloriesEaten << "kcal";
	qDebug() << "Macros -> Protein:" << protein << "g | Carbs:" << carbs << "g | Fats:" << fats << "g";
	qDebug() << "=============================================";
}

