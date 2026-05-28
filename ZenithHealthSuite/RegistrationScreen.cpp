#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"
#include "DatabaseManager.h"
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "DashboardScreen.h"
#include "user.h"
#include "login.h"

RegistrationScreen::RegistrationScreen(QWidget* parent) :
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

RegistrationScreen::~RegistrationScreen()
{
    delete ui;
}

void RegistrationScreen::on_completeRegistrationButton_clicked()
{
    QString AppUsername = ui->usernameLineEdit->text().trimmed();
    QString AppPassword = ui->passwordLineEdit->text();
    QString UserName = ui->namelinedit->text().trimmed();
    QString UserEmail = ui->emailineEdit->text().trimmed();
    QString UserPhone = ui->phoneLineEdit->text().trimmed();

    double UserTargetWeight = ui->targetWeightLineEdit->text().toDouble();
    double UserWeight = ui->weightLineEdit->text().toDouble();
    double UserHeight = ui->heightLineEdit->text().toDouble();

    QString UserGender = ui->genderComboBox->currentText();
    QString UserBloodGroup = ui->bloodComboBox->currentText();
    QString UserDOB = ui->dateEdit->date().toString("dd/MM/yyyy");
    QString UserMedicalHistory = ui->medicalhistrotyLineEdit->toPlainText().trimmed();
    bool hasconsent = ui->consentCheckBox->isChecked();

    if (UserName.isEmpty() || UserEmail.isEmpty() || UserPhone.isEmpty() || AppUsername.isEmpty() || AppPassword.isEmpty()) {
        QMessageBox::warning(this, "Missing Information", "Please fill in all required fields.");
        return;
    }

    if (!hasconsent) {
        QMessageBox::warning(this, "Missing Consent", "Error: You must consent to the terms of service to proceed.");
        return;
    }

    user newUser(
        AppUsername, AppPassword, UserName, UserEmail, UserPhone,
        UserTargetWeight, UserWeight, UserHeight,
        UserGender, UserBloodGroup, UserDOB,
        UserMedicalHistory, hasconsent
    );

    DatabaseManager dbmanager;
    bool issaved = dbmanager.saveUserRegistration(newUser);

    if (issaved) {
        QMessageBox::information(this, "Success", "Registration Successful! Welcome, " + UserName + ".\nPlease login to continue.");

        login* loginScreen = new login();
        loginScreen->show();
        this->close();
    }
    else {
        QMessageBox::critical(this, "Error", "Registration failed. Username or Email might already exist.");
    }
}

void RegistrationScreen::on_btnBackToLogin_clicked()
{
    login* loginscreen = new login();
    loginscreen->show();
    this->close();
}