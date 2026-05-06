
#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include"DatabaseManager.h"
void RegistrationScreen::on_completeRegistration_Clicked() {
	//extract data
	QString username = ui.namelinedit->text();
	QString Email = ui.emailineEdit->text();
	QString contact = ui.phoneLineEdit->text();
	int age = ui.ageLineEdit->text().toInt();
	double weight = ui.weightLineEdit->text().toDouble();
	double height = ui.heightLineEdit->text().toDouble();
	QString gender = ui.genderCombobox->currentText();
	QString medicalHistory = ui.medicalhistrotyLineEdit->text();
	bool consentchecked = ui.consentCheckBox->isChecked();
	//validate feilds
	if (username.isEmpty() || Email.isEmpty() || !consnetChecked) {
		QMessageBox::warning(this, "Registration Error",
			"please fill in all the required fields and accept ZENITH  regulations.");
		return;

	}
	//save to database
	DatabaseManager dbmanager;
	bool issaved = dbmanager.saveUserRegistration(username, Email, contact, age, gender, weight, height, medicalHistory, consentchecked

	);
	if (issaved) {
		QMessageBox::information(this, "Registration Successfull",
			"Your registration was successful! Welcome to the Zenith Health Suite.");
		this->hide();
	}
	else {
		QMessageBox::critical(this, "Database Error",
			"An error occurred while saving your registration. Please try again.");
	}
}




