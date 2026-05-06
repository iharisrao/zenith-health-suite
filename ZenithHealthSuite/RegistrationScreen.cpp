#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"  //it is seceret file which is created by Qt when we use drag and drop canvas
#include <QMessageBox>              // tool which is used to make pop up alerts and show on screen

RegistrationScreen::RegistrationScreen(QWidget *parent):
QMainWindow(parent),
ui(new Ui::RegistrationScreenClass)    //same remote control pointer used in .h file to control the ui elements on the registration screen
{
	ui->setupUi(this);            // magic line which creates the physical lines and boxes on computer screen from your drag and drop design 
}

RegistrationScreen::~RegistrationScreen()
{
	delete ui;
}
void RegistrationScreen::on_completeRegistrationButton_clicked()
{

	QString PatientName = ui->namelinedit->text();
	QString PatientEmail = ui->emailineEdit->text();
	QString PatientPhone = ui->phoneLineEdit->text();
	QString PatientAge = ui->ageLineEdit->text();
	QString PatientGender = ui->genderComboBox->currentText();
	QString PatientHeight = ui->heightLineEdit->text();
	QString PatientWeight = ui->weightLineEdit->text();
	QString PatientMedicalHistory = ui->medicalhistrotyLineEdit->text();
	bool hasconsent = ui->consentCheckBox->isChecked();

	if (PatientName.isEmpty() || PatientEmail.isEmpty() || PatientPhone.isEmpty() || PatientAge.isEmpty() || PatientHeight.isEmpty() || PatientWeight.isEmpty() || PatientMedicalHistory.isEmpty() || PatientGender.isEmpty()) {
		QMessageBox:: warning(this, "Missing Information", "Please fill in all the required fields.");
		return;
	}

	if (hasconsent == false) {
		QMessageBox::warning(this, "Missing Consent", "Error: Patient must consent to HIPAA regulations!");
		return;
	}

	QMessageBox::information(this, "Registration Success", "Data Verified! Ready to save " + PatientName + " to the database.");
	// Pop up a success message box right in the middle of this screen


	ui->namelinedit->clear();
	ui->emailineEdit->clear();
	ui->phoneLineEdit->clear();
	ui->ageLineEdit->clear();
	ui->genderComboBox->setCurrentIndex(0);  //set to zero bcz the box dont know at which value it goes to its initial state
	ui->heightLineEdit->clear();
	ui->weightLineEdit->clear();
	ui->medicalhistrotyLineEdit->clear();
	ui->consentCheckBox->setChecked(false);

}