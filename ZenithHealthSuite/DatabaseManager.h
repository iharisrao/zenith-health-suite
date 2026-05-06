#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>

class {
public:
	DatabaseManager();
		bool openConnection();
	void closeConnection();
	bool saveUserRegiatration(
		const QString& username, const QString& Email, const QString& contact, int age, const QString& gender, double weight, double height,
		const QString& medicalHistory, bool consent
	);
private:
	QSqlDatabase db;
};
#endif // DATABASEMANAGER_H