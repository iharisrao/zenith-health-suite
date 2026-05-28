#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDate>
#include <QList>
#include "user.h" 

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

struct GoalRecord {
    QString targetName;
    QString category;
    double baseline;
    double threshold;
    QString frequency;
};

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool saveUserRegistration(const user& newUser);

    user getUserDetails(const QString& username);

    void createDashboardTables();
    DailyMetrics getDailyMetrics(const QString& username, const QDate& date);
    QList<ActivityRecord> getActivityLog(const QString& username, const QDate& date);

    bool saveActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, int intensity, int caloriesBurned, int steps);

    bool saveVitalsRecord(const QString& username, const QDate& date, double weight, double bodyTemp, int heartRate, int bpSys, int bpDia, int bloodSugar, int stressLevel, double sleepHours);

    bool saveNutritionRecord(const QString& username, const QDate& date, const QString& mealType, const QString& foodName, int calories, int protein, int carbs, int fats, double waterIntake, int caffeine);

    bool saveGoal(const QString& username, const QString& targetName, const QString& category, double baseline, double threshold, const QString& frequency);
    QList<GoalRecord> getGoals(const QString& username);

    bool markDataAsSynced(const QString& username);

private:
    QSqlDatabase db;
    bool setupDatabase();
};
#endif