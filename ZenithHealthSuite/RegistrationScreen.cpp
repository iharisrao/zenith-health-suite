#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"  // it is secret file which is created by Qt when we use drag and drop canvas
#include "DatabaseManager.h"        // Required to save to the database
#include <QMessageBox>              // tool which is used to make pop up alerts and show on screen

// Constructor: We use 'new' here because ui is a pointer in the .h file
RegistrationScreen::RegistrationScreen(QWidget  *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationScreenClass)
{
    ui->setupUi(this);            // magic line which creates the physical lines and boxes on computer screen 
}

// Destructor: We must delete the pointer to prevent memory leaks
RegistrationScreen::~RegistrationScreen()
{
    delete ui;
}

void RegistrationScreen::on_completeRegistrationButton_clicked()
{
    // 1. Extract data - names must match Object Inspector
    QString UserName = ui->namelinedit->text();
    QString UserEmail = ui->emailineEdit->text(); // Corrected from emailineEdit
    QString UserPhone = ui->phoneLineEdit->text();

    // Use value() for spinboxes or text().toInt() for line edits
    int UserAge = ui->ageLineEdit->text().toInt();
    double UserWeight = ui->weightLineEdit->text().toDouble();
    double UserHeight = ui->heightLineEdit->text().toDouble();

    QString UserGender = ui->genderComboBox->currentText(); // Fixed "B" in ComboBox
    QString UserMedicalHistory = ui->medicalhistrotyLineEdit->text(); // Matches your UI typo
    bool hasconsent = ui->consentCheckBox->isChecked();

    // --- Validation Section ---
    if (UserName.isEmpty() || UserEmail.isEmpty() || UserPhone.isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    if (!hasconsent) {
        QMessageBox::warning(this, "Missing Consent", "Error: user must consent to regulations!");
        return;
    }

    // --- Database Section ---
    
    DatabaseManager dbmanager;
    bool issaved = dbmanager.saveUserRegistration(
        UserName, UserEmail, UserPhone, UserAge,
        UserGender, UserWeight, UserHeight, UserMedicalHistory, hasconsent
    );
    
	 // Placeholder for testing without database
    if (issaved) {
        QMessageBox::information(this, "Success", "Data saved for " + UserName);
        this->hide();
    }
    else {
        QMessageBox::critical(this, "Error", "Database error occurred.");
    }
}