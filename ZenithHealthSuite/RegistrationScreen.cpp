
#include "RegistrationScreen.h"

RegistrationScreen::RegistrationScreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

RegistrationScreen::~RegistrationScreen()
{}

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