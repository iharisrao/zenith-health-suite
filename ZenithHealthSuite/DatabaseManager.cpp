#include "DatabaseManager.h"
#include <QDebug>
#include <QCryptographicHash>
#include <QVariant>
#include <QSqlError>
#include <QCoreApplication> 
#include <QMessageBox>

DatabaseManager::DatabaseManager()
{
    setupDatabase();
}

DatabaseManager::~DatabaseManager()
{
    
}

bool DatabaseManager::setupDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE");

        QString databasePath = QCoreApplication::applicationDirPath() + "/ZenithHealth.db";
        db.setDatabaseName(databasePath);

        qDebug() << "Database initialized at: " << databasePath;
    }

    if (!db.open()) {
        qDebug() << "Error: Connection with database failed";
        return false;
    }

    db.transaction();

    QSqlQuery query(db);

    if (!query.exec(R"(
        CREATE TABLE IF NOT EXISTS User (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE,
            password TEXT,
            name TEXT,
            email TEXT UNIQUE,
            phone TEXT,
            targetWeight REAL,
            weight REAL,
            height REAL,
            gender TEXT,
            bloodGroup TEXT,
            dob TEXT,
            medicalHistory TEXT,
            consent BOOLEAN
        )
    )")) {
        db.rollback();
        return false;
    }

    db.commit();

    createDashboardTables();
    return true;
}

bool DatabaseManager::saveUserRegistration(const user& newUser)
{
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "The database is not open.");
        return false;
    }

    QString hashedPassword = QString(QCryptographicHash::hash(
        newUser.getPassword().toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query(db);

    query.prepare(R"(
        INSERT INTO User (username, password, name, email, phone, targetWeight, weight, height, gender, bloodGroup, dob, medicalHistory, consent) 
        VALUES (:username, :password, :name, :email, :phone, :targetWeight, :weight, :height, :gender, :bloodGroup, :dob, :medicalHistory, :consent)
    )");

    query.bindValue(":username", newUser.getUsername());
    query.bindValue(":password", hashedPassword);
    query.bindValue(":name", newUser.getName());
    query.bindValue(":email", newUser.getEmail());
    query.bindValue(":phone", newUser.getPhone());
    query.bindValue(":targetWeight", newUser.getTargetWeight());
    query.bindValue(":weight", newUser.getWeight());
    query.bindValue(":height", newUser.getHeight());
    query.bindValue(":gender", newUser.getGender());
    query.bindValue(":bloodGroup", newUser.getBloodGroup());
    query.bindValue(":dob", newUser.getDob());
    query.bindValue(":medicalHistory", newUser.getMedicalHistory());
    query.bindValue(":consent", newUser.getConsent() ? 1 : 0);

    // SQL Error Trap to catch specific rejection reasons
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Registration Failed", "The database rejected the data. Reason:\n\n" + query.lastError().text());
        return false;
    }

    return true;
}

user DatabaseManager::getUserDetails(const QString& username)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM User WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        return user(
            query.value("username").toString(),
            query.value("password").toString(),
            query.value("name").toString(),
            query.value("email").toString(),
            query.value("phone").toString(),
            query.value("targetWeight").toDouble(),
            query.value("weight").toDouble(),
            query.value("height").toDouble(),
            query.value("gender").toString(),
            query.value("bloodGroup").toString(),
            query.value("dob").toString(),
            query.value("medicalHistory").toString(),
            query.value("consent").toBool()
        );
    }
    return user();
}

void DatabaseManager::createDashboardTables()
{
    db.transaction();
    QSqlQuery query(db);

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS DailyMetrics (
            metric_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            record_date TEXT,
            calories_consumed INTEGER,
            calories_goal INTEGER,
            protein_g INTEGER,
            carbs_g INTEGER,
            fats_g INTEGER,
            active_energy INTEGER,
            step_count INTEGER,
            sleep_hours INTEGER,
            sleep_minutes INTEGER,
            recovery_percentage INTEGER,
            heart_rate INTEGER,
            hydration_status TEXT,
            recovery_status TEXT,
            stress_status TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS ActivityLog(
            activity_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            record_date TEXT,
            exercise_name TEXT,
            duration_mins INTEGER,
            intensity INTEGER,
            calories_burned INTEGER,
            step_count INTEGER,
            sync_status TEXT
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS VitalsLog(
            vital_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            record_date TEXT,
            weight REAL,
            body_temp REAL,
            heart_rate INTEGER,
            bp_sys INTEGER,
            bp_dia INTEGER,
            blood_sugar INTEGER,
            stress_level INTEGER,
            sleep_hours REAL
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS NutritionLog(
            nutrition_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            record_date TEXT,
            meal_type TEXT,
            food_name TEXT,
            calories INTEGER,
            protein_g INTEGER,
            carbs_g INTEGER,
            fats_g INTEGER,
            water_l REAL,
            caffeine_cups INTEGER
        )
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS UserGoals(
            goal_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            target_name TEXT,
            category TEXT,
            baseline REAL,
            threshold REAL,
            frequency TEXT
        )
    )");

    db.commit();
}

DailyMetrics DatabaseManager::getDailyMetrics(const QString& username, const QDate& date)
{
    DailyMetrics data = { 0 };
    QString dateStr = date.toString("yyyy-MM-dd");

    QSqlQuery nutritionQuery(db);
    nutritionQuery.prepare(R"(
        SELECT SUM(calories), SUM(protein_g), SUM(carbs_g), SUM(fats_g) 
        FROM NutritionLog WHERE username = :usr AND record_date = :date
    )");
    nutritionQuery.bindValue(":usr", username);
    nutritionQuery.bindValue(":date", dateStr);

    if (nutritionQuery.exec() && nutritionQuery.next()) {
        data.caloriesConsumed = nutritionQuery.value(0).toInt();
        data.protien = nutritionQuery.value(1).toInt();
        data.carbs = nutritionQuery.value(2).toInt();
        data.fats = nutritionQuery.value(3).toInt();
    }

    QSqlQuery activityQuery(db);
    activityQuery.prepare(R"(
        SELECT SUM(calories_burned), SUM(step_count) FROM ActivityLog 
        WHERE username = :usr AND record_date = :date
    )");
    activityQuery.bindValue(":usr", username);
    activityQuery.bindValue(":date", dateStr);

    if (activityQuery.exec() && activityQuery.next()) {
        data.activeEnergy = activityQuery.value(0).toInt();
        data.stepCount = activityQuery.value(1).toInt();
    }

    QSqlQuery vitalsQuery(db);
    vitalsQuery.prepare(R"(
        SELECT heart_rate, sleep_hours FROM VitalsLog 
        WHERE username = :usr AND record_date = :date ORDER BY vital_id DESC LIMIT 1
    )");
    vitalsQuery.bindValue(":usr", username);
    vitalsQuery.bindValue(":date", dateStr);

    if (vitalsQuery.exec() && vitalsQuery.next()) {
        data.heartRate = vitalsQuery.value(0).toInt();
        double totalSleep = vitalsQuery.value(1).toDouble();
        data.sleepHours = static_cast<int>(totalSleep);
        data.sleepMinutes = static_cast<int>((totalSleep - data.sleepHours) * 60);

        double recPct = (totalSleep / 8.0) * 100.0;
        data.recoveryPercentage = (recPct > 100.0) ? 100 : static_cast<int>(recPct);
    }

    return data;
}

QList<ActivityRecord> DatabaseManager::getActivityLog(const QString& username, const QDate& date)
{
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
    return logList;
}

bool DatabaseManager::saveActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, int intensity, int caloriesBurned, int steps)
{
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO ActivityLog (username, record_date, exercise_name, duration_mins, intensity, calories_burned, step_count, sync_status) 
        VALUES (:username, :date, :exercise, :duration, :intensity, :calories, :steps, 'PENDING')
    )");

    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":exercise", exerciseName);
    query.bindValue(":duration", durationMins);
    query.bindValue(":intensity", intensity);
    query.bindValue(":calories", caloriesBurned);
    query.bindValue(":steps", steps);

    return query.exec();
}

bool DatabaseManager::saveVitalsRecord(const QString& username, const QDate& date, double weight, double bodyTemp, int heartRate, int bpSys, int bpDia, int bloodSugar, int stressLevel, double sleepHours)
{
    QSqlQuery weightQuery(db);
    weightQuery.prepare("UPDATE User SET weight = :weight WHERE username = :username");
    weightQuery.bindValue(":weight", weight);
    weightQuery.bindValue(":username", username);
    weightQuery.exec();

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO VitalsLog (username, record_date, weight, body_temp, heart_rate, bp_sys, bp_dia, blood_sugar, stress_level, sleep_hours) 
        VALUES (:username, :date, :weight, :temp, :hr, :bpsys, :bpdia, :sugar, :stress, :sleep)
    )");

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

    return query.exec();
}

bool DatabaseManager::saveNutritionRecord(const QString& username, const QDate& date, const QString& mealType, const QString& foodName, int calories, int protein, int carbs, int fats, double waterIntake, int caffeine)
{
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO NutritionLog (username, record_date, meal_type, food_name, calories, protein_g, carbs_g, fats_g, water_l, caffeine_cups) 
        VALUES (:username, :date, :meal, :food, :cals, :pro, :carbs, :fats, :water, :caf)
    )");

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

    return query.exec();
}

bool DatabaseManager::saveGoal(const QString& username, const QString& targetName, const QString& category, double baseline, double threshold, const QString& frequency)
{
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO UserGoals (username, target_name, category, baseline, threshold, frequency)
        VALUES (:username, :targetName, :category, :baseline, :threshold, :frequency)
    )");

    query.bindValue(":username", username);
    query.bindValue(":targetName", targetName);
    query.bindValue(":category", category);
    query.bindValue(":baseline", baseline);
    query.bindValue(":threshold", threshold);
    query.bindValue(":frequency", frequency);

    return query.exec();
}

QList<GoalRecord> DatabaseManager::getGoals(const QString& username)
{
    QList<GoalRecord> goalsList;
    QSqlQuery query(db);

    query.prepare("SELECT target_name, category, baseline, threshold, frequency FROM UserGoals WHERE username=:username");
    query.bindValue(":username", username);

    if (query.exec()) {
        while (query.next()) {
            GoalRecord record;
            record.targetName = query.value("target_name").toString();
            record.category = query.value("category").toString();
            record.baseline = query.value("baseline").toDouble();
            record.threshold = query.value("threshold").toDouble();
            record.frequency = query.value("frequency").toString();
            goalsList.append(record);
        }
    }
    return goalsList;
}

bool DatabaseManager::markDataAsSynced(const QString& username)
{
    QSqlQuery query(db);
    query.prepare("UPDATE ActivityLog SET sync_status = 'SYNCED' WHERE username = :usr AND sync_status = 'PENDING'");
    query.bindValue(":usr", username);

    return query.exec();
}