#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"  // it is secret file which is created by Qt when we use drag and drop canvas
#include "DatabaseManager.h"        // Required to save to the database
#include <QMessageBox>          // tool which is used to make pop up alerts and show on screen
#include <QGraphicsDropShadowEffect>

// Constructor: We use 'new' here because ui is a pointer in the .h file
RegistrationScreen::RegistrationScreen(QWidget  *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationScreenClass)
{
    ui->setupUi(this);            
    QGraphicsDropShadowEffect* shadow1 = new QGraphicsDropShadowEffect(this);
    shadow1->setBlurRadius(30);
    shadow1->setColor(QColor(0, 0, 0, 15)); 
    shadow1->setOffset(0, 4); 
    ui->card1->setGraphicsEffect(shadow1);

    QGraphicsDropShadowEffect* shadow2 = new QGraphicsDropShadowEffect(this);
    shadow2->setBlurRadius(30);
    shadow2->setColor(QColor(0, 0, 0, 15));
    shadow2->setOffset(0, 4);
    ui->card2->setGraphicsEffect(shadow2);

    QGraphicsDropShadowEffect* shadow3 = new QGraphicsDropShadowEffect(this);
    shadow3->setBlurRadius(30);
    shadow3->setColor(QColor(0, 0, 0, 15));
    shadow3->setOffset(0, 4);
    ui->card3->setGraphicsEffect(shadow3);
}

// Destructor: We must delete the pointer to prevent memory leaks
RegistrationScreen::~RegistrationScreen()
{
    delete ui;
}

void RegistrationScreen::on_completeRegistrationButton_clicked()
{
    // 1. Extract data - names must match Object Inspector
    QString AppUsername = ui->usernameLineEdit->text();
    QString AppPassword = ui->passwordLineEdit->text();
    QString UserName = ui->namelinedit->text();
    QString UserEmail = ui->emailineEdit->text(); // Corrected from emailineEdit
    QString UserPhone = ui->phoneLineEdit->text();


    double UserTargetWeight = ui->targetWeightLineEdit->text().toDouble();
    double UserWeight = ui->weightLineEdit->text().toDouble();
    double UserHeight = ui->heightLineEdit->text().toDouble();

    QString UserGender = ui->genderComboBox->currentText(); 
    QString UserBloodGroup = ui->bloodComboBox->currentText();
    QString UserDOB = ui->dateEdit->date().toString("dd/MM/yyyy");
    QString UserMedicalHistory = ui->medicalhistrotyLineEdit->toPlainText(); 
    bool hasconsent = ui->consentCheckBox->isChecked();

    if (UserName.isEmpty() || UserEmail.isEmpty() || UserPhone.isEmpty() || AppUsername.isEmpty() || AppPassword.isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    if (!hasconsent) {
        QMessageBox::warning(this, "Missing Consent", "Error: user must consent to regulations!");
        return;
    }


    
    DatabaseManager dbmanager;
    bool issaved = dbmanager.saveUserRegistration(
        AppUsername, AppPassword, UserName, UserEmail, UserPhone,
        UserTargetWeight, UserWeight, UserHeight,
        UserGender, UserBloodGroup, UserDOB,
        UserMedicalHistory, hasconsent
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