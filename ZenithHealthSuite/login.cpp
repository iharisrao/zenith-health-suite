#include "login.h"
#include"ui_login.h"
#include"RegistrationScreen.h"
#include"DashboardScreen.h"

#include<QMessageBox>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QCryptographicHash>

login::login(QWidget *parent)
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
	QString username = ui->usernameLineEdit->text();
	QString password = ui->passwordLineEdit->text();

	QString hashedInputPassword = QString(QCryptographicHash::hash(
		password.toUtf8(), QCryptographicHash::Sha256).toHex());

	if (username.isEmpty() || password.isEmpty()) {
		QMessageBox::warning(this, "Input Required", "Please Enter both Username and Password");
		return;
	}
	QSqlDatabase db = QSqlDatabase::database();
	if (!db.isValid() || !db.isOpen()) {
		db = QSqlDatabase::addDatabase("QSQLITE");
		db.setDatabaseName("ZenithHealth.db");
		if(!db.isOpen()){
			QMessageBox::critical(this, "Database Error", "Connection with database failed!");
			return;
		}
	}

	QSqlQuery query(db);
	query.prepare("Select * from User where username=:username and password=:password");
	query.bindValue(":username", username);
	query.bindValue(":password", hashedInputPassword);

	if (query.exec()) {
		if (query.next()) {
			QMessageBox::information(this,"Success", "Login Successful! Opening Dashboard.....");
			DashboardScreen* dash = new DashboardScreen();
			dash->show();
			this->close();
		}
		else {
			QMessageBox::critical(this, "Query Error", "Database error: " + query.lastError().text());
		}
	}
}

void login::on_signUpButton_clicked(){
	RegistrationScreen *regWindow = new RegistrationScreen();
	regWindow-> show();
	this->close();

}

