#include "DatabaseManager.h"
#include<QDebug>

DatabaseManager::DatabaseManager()

{
	setupDatabase();
}
DatabaseManager::~DatabaseManager()
{
	if (db.isOpen()) {
		db.close();
	}
}
bool DatabaseManager::setupDatabase()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("ZenithHealth.db");
	if (!db.open()) {
		qDebug() << "Error:Connection with database failed";
		return false;
	}
	QSqlQuery query;
	query.prepare("CREATE TABLE IF NOT EXISTS User ("
	"id INTEGER PRIMARY KEY AUTOINCREMENT,"
	"name TEXT,"
	"email TEXT,"
	"phone TEXT,"
	"age INTEGER,"
	"gender TEXT,"
	"weight REAL,"
	"height REAL,"
	"medicalHistory TEXT,"
	"consent BOOLEAN"
	")");
	if (!query.exec()) {
		qDebug() << "Error: creating Table -" << query.lastError();
		return false;
	}
	return true;
}
bool DatabaseManager::saveUserRegistration(const QString& name, const QString& email, const QString& phone, int age, const QString& gender,
	double weight, double height, const QString& medicalhistory, bool consent)
{
	if (!db.isOpen()) {
		qDebug() << "Database is not open!";
		return false;
	}
	QSqlQuery query(" INSERT INTO User (name,email,phone,age,gender,weight,height,medicalhistory,consent)"
		"Values(:name,:email,:phone,:age,:gender,:weight,:height,:medicalhistory,:consnet)");

	query.bindValue(":name:", name);
	query.bindValue(":email:", email);
	query.bindValue(":phone", phone);
	query.bindValue(":age", age);
	query.bindValue(":gender", gender);
	query.bindValue(":wieght", weight);
	query.bindValue(":height", height);
	query.bindValue(":medicalhistory", medicalhistory);
	query.bindValue(":consnet", consent ? 1 : 0);
	if (!query.exec()) {
		qDebug() << "Error: inserting Data" << query.lastError();
		return false;
	}
	return true;
}