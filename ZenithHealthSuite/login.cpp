#include "login.h"
#include "ui_login.h"
#include "RegistrationScreen.h"
#include "DashboardScreen.h"
#include "user.h" 
#include <QMessageBox>
#include <QCryptographicHash>

login::login(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("ZenithHealthSuite - Login Portal");
}

login::~login()
{
    delete ui;
}

void login::on_signInButton_clicked()
{

    QString username = ui->usernameLineEdit->text().trimmed();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Required", "Please enter both Username and Password.");
        return;
    }

    QString hashedInputPassword = QString(QCryptographicHash::hash(
        password.toUtf8(), QCryptographicHash::Sha256).toHex());

    user loggedInUser = dbManager.getUserDetails(username);

    if (loggedInUser.getUsername().isEmpty()) {
        QMessageBox::critical(this, "Database Error",
            "This Username is not in the database!\n\nPlease check your spelling or register a new account.");
        return;
    }

    if (loggedInUser.getPassword() != hashedInputPassword) {
        QMessageBox::critical(this, "Password Error",
            "Username found, but the password does not match!");
        return;
    }

    QMessageBox::information(this, "Success", "Login Successful! Welcome, " + loggedInUser.getName() + ".");

    DashboardScreen* dash = new DashboardScreen(loggedInUser);
    dash->show();
    this->close();
}

void login::on_signUpButton_clicked()
{
    RegistrationScreen* regWindow = new RegistrationScreen();
    regWindow->show();
    this->close();
}