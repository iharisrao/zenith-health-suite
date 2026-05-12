#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QString>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include<QDate>
#include<QList>

//Dashboard structure
struct DailyMetrics {
	int caloriesConsumed;
	int caloriesGoal;
	int protien;
	int carbs;
	int fats;
	int activeEnergy;
	int stepCount;
	int sleepHours;
	int sleepMinutes;
	int recoveryPercentage;
	int heartRate;
	QString hydrationStatus;
	QString recoveryStatus;
	QString stressStatus;
};

struct ActivityRecord {
	QString exerciseName;
	int durationMins;
	int caloriesBurned;
	QString intensity;
	QString syncStatus;
};

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
	//////new functions
	void createDashboardTables();
	DailyMetrics getDailyMetrics(const QString&  username, const QDate& date);
	QList<ActivityRecord> getActivityLog(const QString& username, const QDate& date);

	bool addActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, const QString& intensity, int caloriesBurned);
	bool saveNutritionData(const QString& username, const QDate& date, int calories, int protien, int carbs, int fats);
	bool saveVitalsData(const QString& username, const QDate& date, int steps, double sleepHours, double waterIntake, int heartRate, double weight);

private:
	QSqlDatabase db;
	bool setupDatabase();
	
};
#endif // DATABASEMANAGER_H
