#include "DatabaseManager.h"
#include <QDebug>
#include <QCryptographicHash>   //to protect password 
#include<QVariant> // help us convert data base valuues into int
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
//////DASHBOARD FUNCTION START HERE//////////
void DatabaseManager::createDashboardTables() {
    QSqlQuery query;
    //creating dailymetrics tables for the top cards
    QString createDailyMetricsQuery =
        "CREATE TABLE IF NOT EXISTS DailyMetrics ("
        "metric_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "usename TEXT,"
        "record_data TEXT,"
        "calories_consumed INTEGER,"
        "calories_goal INTEGER,"
        "protien_g INTEGER,"
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
        "stress_status TEXT,"
       ")";
        

    if (!query.exec(createDailyMetricsQuery)) {
        qDebug() << "Failed to create DailyMetrics table:" << query.lastError().text();
        
    }
    ///create the activitylog table for the bottom table weight
    QString createActivityQuery =
        "CREATE TABLE IF NOT EXIST ActivityLog("
        "activity_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_name TEXT,"
        "record_date TEXT,"
        "exercise_name TEXT,"
        "duration_mins INTEGER,"
        "intensity TEXT,"
        "calories_burned INTEGER,"
        "sync_status TEXT )";
    if (!query.exec(createActivityQuery)) {
        qDebug() << " Failed to create ActivityLog table:" << query.lastError().text();
        
    }
 
}
DailyMetrics DatabaseManager::getDailyMetrics(const QString& username, const QDate& date) {
    DailyMetrics data = { 0 };
    QSqlQuery query (db);
    query.prepare("Select*from DailyMetrics where username= :username AND record_date=:date");
    query.bindValue(":username", username);
    query.bindValue(":date" ,date.toString("yyyy_MM_dd"));
    if (query.exec() && query.next()) {
        data.caloriesConsumed = query.value("calories_goal").toInt();
        data.protien = query.value("protien_g").toInt();
        data.carbs = query.value("carbs_g").toInt();
        data.fats = query.value("fats_g").toInt();
        data.activeEnergy = query.value("active_energy").toInt();
        data.stepCount = query.value("step_Count").toInt();
        data.sleepHours = query.value("sleep_Hours").toInt();
        data.sleepMinutes = query.value("sleep_minutes").toInt();
        data.recoveryPercentage = query.value("recovery_perecentage").toInt();
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
QList<ActivityRecord>DatabaseManager::getActivityLog(const QString& username, const QDate& date){
    QList<ActivityRecord>logList;
    QSqlQuery query;

    query.prepare("select * from ActivityLog WHERE username=:username and record_date=:date");
    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    if (!query.exec()) {
        while (query.next()) {
            ActivityRecord record;
            record.exerciseName = query.value("exercise_name").toString();
            record.durationMins = query.value("duration_mins").toInt();
            record.intensity = query.value("intensity").toString();
            record.caloriesBurned = query.value("caloriesburned").toInt();
            record.syncStatus = query.value("sync_status").toString();

            logList.append(record);
        }
    }
    else {
        qDebug() << "Failed to fetch activity log:" << query.lastError().text();
    }
    return logList;
}

