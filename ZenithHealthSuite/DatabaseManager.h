#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDate>
#include <QList>

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
        const QString& username, const QString& password, const QString& name,
        const QString& email, const QString& contact, double targetWeight,
        double weight, double height, const QString& gender,
        const QString& bloodGroup, const QString& dob,
        const QString& medicalHistory, bool consent
    );


    void createDashboardTables();
    DailyMetrics getDailyMetrics(const QString& username, const QDate& date);
    QList<ActivityRecord> getActivityLog(const QString& username, const QDate& date);

    bool saveActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, int intensity, int caloriesBurned);

    bool saveVitalsRecord(const QString& username, const QDate& date, double weight, double bodyTemp, int heartRate, int bpSys, int bpDia, int bloodSugar, int stressLevel, int sleepHours);

    bool saveNutritionRecord(const QString& username, const QDate& date, const QString& mealType, const QString& foodName, int calories, int protein, int carbs, int fats, int waterIntake, int caffeine);

private:
    QSqlDatabase db;
    bool setupDatabase();
};

#endif