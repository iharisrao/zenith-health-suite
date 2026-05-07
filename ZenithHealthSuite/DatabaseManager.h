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
		const QString& password,
		const QString& name,
		const QString& email,
		const QString& contact,
		double targetWeight,
		double weight,
		double height,
		const QString& gender,
		const QString& bloodGroup,
		const QString& dob,
		const QString& medicalHistory,
		bool consent
	);
private:
	QSqlDatabase db;
	bool setupDatabase();
	
};
#endif // DATABASEMANAGER_H