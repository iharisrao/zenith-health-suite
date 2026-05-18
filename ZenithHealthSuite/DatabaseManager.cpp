#include "DatabaseManager.h"
#include <QDebug>
#include <QCryptographicHash>   
#include <QVariant>           

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
        qDebug() << "Error: Connection with database failed";
        return false;
    }

    QSqlQuery query(db);

    if (!query.prepare("CREATE TABLE IF NOT EXISTS User ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE,"
        "password TEXT,"
        "name TEXT,"
        "email TEXT UNIQUE,"
        "phone TEXT,"
        "targetWeight REAL,"
        "weight REAL,"
        "height REAL,"
        "gender TEXT,"
        "bloodGroup TEXT,"
        "dob TEXT,"
        "medicalHistory TEXT,"
        "consent BOOLEAN"
        ")")) {
        qDebug() << "Error: preparing Table -" << query.lastError();
        return false;
    }

    if (!query.exec()) {
        qDebug() << "Error: creating Table -" << query.lastError();
        return false;
    }
    return true;
}

bool DatabaseManager::saveUserRegistration(const QString& username, const QString& password, const QString& name, const QString& email, const QString& phone, double targetWeight, double weight, double height, const QString& gender, const QString& bloodGroup, const QString& dob, const QString& medicalhistory, bool consent)
{
    if (!db.isOpen()) {
        qDebug() << "Database is not open!";
        return false;
    }

    QString hashedPassword = QString(QCryptographicHash::hash(
        password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query(db);

    if (!query.prepare("INSERT INTO User (username, password, name, email, phone, targetWeight, weight, height, gender, bloodGroup, dob, medicalHistory, consent) "
        "VALUES (:username, :password, :name, :email, :phone, :targetWeight, :weight, :height, :gender, :bloodGroup, :dob, :medicalHistory, :consent)")) {
        qDebug() << "Error: preparing insert -" << query.lastError();
        return false;
    }

    query.bindValue(":username", username);
    query.bindValue(":password", hashedPassword);
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":targetWeight", targetWeight);
    query.bindValue(":weight", weight);
    query.bindValue(":height", height);
    query.bindValue(":gender", gender);
    query.bindValue(":bloodGroup", bloodGroup);
    query.bindValue(":dob", dob);
    query.bindValue(":medicalHistory", medicalhistory);
    query.bindValue(":consent", consent ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "Error: inserting Data" << query.lastError();
        return false;
    }
    return true;
}

void DatabaseManager::createDashboardTables() {
    QSqlQuery query(db);

    QString createDailyMetricsQuery =
        "CREATE TABLE IF NOT EXISTS DailyMetrics ("
        "metric_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "record_date TEXT,"
        "calories_consumed INTEGER,"
        "calories_goal INTEGER,"
        "protein_g INTEGER,"
        "carbs_g INTEGER,"
        "fats_g INTEGER,"
        "active_energy INTEGER,"
        "step_count INTEGER,"
        "sleep_hours INTEGER,"
        "sleep_minutes INTEGER,"
        "recovery_percentage INTEGER,"
        "heart_rate INTEGER,"
        "hydration_status TEXT,"
        "recovery_status TEXT,"
        "stress_status TEXT)";

    if (!query.exec(createDailyMetricsQuery)) {
        qDebug() << "Failed to create DailyMetrics table:" << query.lastError().text();
    }

    QString createActivityQuery =
        "CREATE TABLE IF NOT EXISTS ActivityLog("
        "activity_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "record_date TEXT,"
        "exercise_name TEXT,"
        "duration_mins INTEGER,"
        "intensity INTEGER,"
        "calories_burned INTEGER,"
        "sync_status TEXT)";

    if (!query.exec(createActivityQuery)) {
        qDebug() << "Failed to create ActivityLog table:" << query.lastError().text();
    }

    QString createVitalsQuery =
        "CREATE TABLE IF NOT EXISTS VitalsLog("
        "vital_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "record_date TEXT,"
        "weight REAL,"
        "body_temp REAL,"
        "heart_rate INTEGER,"
        "bp_sys INTEGER,"
        "bp_dia INTEGER,"
        "blood_sugar INTEGER,"
        "stress_level INTEGER,"
        "sleep_hours INTEGER)";

    if (!query.exec(createVitalsQuery)) {
        qDebug() << "Failed to create VitalsLog table:" << query.lastError().text();
    }

    QString createNutritionQuery =
        "CREATE TABLE IF NOT EXISTS NutritionLog("
        "nutrition_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "record_date TEXT,"
        "meal_type TEXT,"
        "food_name TEXT,"
        "calories INTEGER,"
        "protein_g INTEGER,"
        "carbs_g INTEGER,"
        "fats_g INTEGER,"
        "water_l INTEGER,"
        "caffeine_cups INTEGER)";

    if (!query.exec(createNutritionQuery)) {
        qDebug() << "Failed to create NutritionLog table:" << query.lastError().text();
    }
}

DailyMetrics DatabaseManager::getDailyMetrics(const QString& username, const QDate& date) {
    DailyMetrics data = { 0 };
    QSqlQuery query(db);
    query.prepare("SELECT * FROM DailyMetrics WHERE username = :username AND record_date = :date");
    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (query.exec() && query.next()) {
        data.caloriesConsumed = query.value("calories_consumed").toInt();
        data.protien = query.value("protein_g").toInt();
        data.carbs = query.value("carbs_g").toInt();
        data.fats = query.value("fats_g").toInt();
        data.activeEnergy = query.value("active_energy").toInt();
        data.stepCount = query.value("step_count").toInt();
        data.sleepHours = query.value("sleep_hours").toInt();
        data.sleepMinutes = query.value("sleep_minutes").toInt();
        data.recoveryPercentage = query.value("recovery_percentage").toInt();
        data.heartRate = query.value("heart_rate").toInt();
        data.hydrationStatus = query.value("hydration_status").toString();
        data.recoveryStatus = query.value("recovery_status").toString();
        data.stressStatus = query.value("stress_status").toString();
    }
    else {
        qDebug() << "No metrics found for user:" << username << "on date:" << date.toString("yyyy-MM-dd");
    }
    return data;
}

QList<ActivityRecord> DatabaseManager::getActivityLog(const QString& username, const QDate& date) {
    QList<ActivityRecord> logList;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM ActivityLog WHERE username = :username AND record_date = :date");
    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (query.exec()) {
        while (query.next()) {
            ActivityRecord record;
            record.exerciseName = query.value("exercise_name").toString();
            record.durationMins = query.value("duration_mins").toInt();
            record.intensity = query.value("intensity").toString();
            record.caloriesBurned = query.value("calories_burned").toInt();
            record.syncStatus = query.value("sync_status").toString();

            logList.append(record);
        }
    }
    else {
        qDebug() << "Failed to fetch activity log:" << query.lastError().text();
    }
    return logList;
}

bool DatabaseManager::saveActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, int intensity, int caloriesBurned)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO ActivityLog (username, record_date, exercise_name, duration_mins, intensity, calories_burned, sync_status) "
        "VALUES (:username, :date, :exercise, :duration, :intensity, :calories, 'PENDING')");

    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":exercise", exerciseName);
    query.bindValue(":duration", durationMins);
    query.bindValue(":intensity", intensity);
    query.bindValue(":calories", caloriesBurned);

    if (!query.exec()) {
        qDebug() << "Error Saving Activity:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::saveVitalsRecord(const QString& username, const QDate& date, double weight, double bodyTemp, int heartRate, int bpSys, int bpDia, int bloodSugar, int stressLevel, int sleepHours)
{

    QSqlQuery weightQuery(db);
    weightQuery.prepare("UPDATE User SET weight = :weight WHERE username = :username");
    weightQuery.bindValue(":weight", weight);
    weightQuery.bindValue(":username", username);
    weightQuery.exec();

    QSqlQuery query(db);
    query.prepare("INSERT INTO VitalsLog (username, record_date, weight, body_temp, heart_rate, bp_sys, bp_dia, blood_sugar, stress_level, sleep_hours) "
        "VALUES (:username, :date, :weight, :temp, :hr, :bpsys, :bpdia, :sugar, :stress, :sleep)");

    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":weight", weight);
    query.bindValue(":temp", bodyTemp);
    query.bindValue(":hr", heartRate);
    query.bindValue(":bpsys", bpSys);
    query.bindValue(":bpdia", bpDia);
    query.bindValue(":sugar", bloodSugar);
    query.bindValue(":stress", stressLevel);
    query.bindValue(":sleep", sleepHours);

    if (!query.exec()) {
        qDebug() << "Error Saving Vitals:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::saveNutritionRecord(const QString& username, const QDate& date, const QString& mealType, const QString& foodName, int calories, int protein, int carbs, int fats, int waterIntake, int caffeine)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO NutritionLog (username, record_date, meal_type, food_name, calories, protein_g, carbs_g, fats_g, water_l, caffeine_cups) "
        "VALUES (:username, :date, :meal, :food, :cals, :pro, :carbs, :fats, :water, :caf)");

    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":meal", mealType);
    query.bindValue(":food", foodName);
    query.bindValue(":cals", calories);
    query.bindValue(":pro", protein);
    query.bindValue(":carbs", carbs);
    query.bindValue(":fats", fats);
    query.bindValue(":water", waterIntake);
    query.bindValue(":caf", caffeine);

    if (!query.exec()) {
        qDebug() << "Error Saving Nutrition:" << query.lastError().text();
        return false;
    }
    return true;
}