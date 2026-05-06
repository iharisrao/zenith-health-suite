#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include <QSqlDatabase>

class {
public:
	DATABASEMANAGER()
		bool openConnection();
	void closeConnection();
	bool saveUserRegiatration(
		const QString& fullname, const QString& email, const QString& Email, const QString& contact, int age, const QString& gender, double weight, double height,
		const QString& medicalHistory, bool consent
	);
private:
	QSqlDatabase db;
};
#endif // DATABASEMANAGER_H