#include "DataEntryDialog.h"
#include "ui_DataEntryDialog.h"

DataEntryDialog::DataEntryDialog(QWidegt* parent);
QDialog(parent),
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

void DatantryDialog::on_buttonBox_accepted() {
	QString logDate = ui->logDateEdit->date().toString("yyyy-MM-dd");

	QString exercise = ui->exerciseComboBox->currenttext();

}

