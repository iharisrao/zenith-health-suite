#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>

class DatabaseManager {
public:
	DatabaseManager();
	~DatabaseManager();

	bool saveUserRegistration(
		const QString& username, 
		const QString& Email, 
		const QString& contact, 
		int age, 
		const QString& gender, 
		double weight, 
		double height,
		const QString& medicalHistory, 
		bool consent
	);
private:
	QSqlDatabase db;
	bool setupDatabase();
};
#endif // DATABASEMANAGER_H