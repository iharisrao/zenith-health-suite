#include "DatabaseManager.h"    //coonect header file to our c++ file 
#include <QDebug>  //QDebug is for printing messages to the console specifcally used for debugging purposes
#include <QCryptographicHash>  // save password in encrypted form in database 
#include <QVariant>      // QVareint is special datatype which can hold any type of data and we can convert it to any type we want
#include <QSqlError>   //gves u access to .lastError() so  find out the last error occur in our database 
#include <QCoreApplication>   //helps u to find out .exe file location and create database there
#include <QMessageBox>  //used to show pop up window on screen 

DatabaseManager::DatabaseManager()
{
	setupDatabase();     /* when the constructor runs it automatically calls the setupDatabase function to
                           initialize the database connection and create necessary tables if they don't exist. 
                           This ensures that the database is ready for use as soon as an instance of DatabaseManager
                           is created */
}

DatabaseManager::~DatabaseManager()
{
    //destructor which is empty bcz database automatically closes itself when the app closes 
}

bool DatabaseManager::setupDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {          //qt_sql_default_connection is default name of databaase which QT creates
        db = QSqlDatabase::database("qt_sql_default_connection");
        //if default database exist we use it , this precents crashes of duplicate connections
    }
    else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        // it default database doesnot exists we will create new one and here QSQLITE tells QT that we are using SQLite database driver

        QString databasePath = QCoreApplication::applicationDirPath() + "/ZenithHealth.db"; //dynamically telling compiler to find database file on its own
        db.setDatabaseName(databasePath);

		// creating database full path by combining the application directory path and the database file name "ZenithHealth.db". This ensures that the database file is created in the same directory as the application executable, making it easier to manage and access.

        qDebug() << "Database initialized at: " << databasePath;
        // prints the path to the debug console it is useful during development to confirm the database file was found at right location 
    }

    if (!db.open()) {
        qDebug() << "Error: Connection with database failed";  // if the database doesnot open it will show an error 
        return false;
    }

    db.transaction(); // tells compiler to group all database operation runs them if any of them fails undo all of them 

    QSqlQuery query(db); // act as sql command runner 

    // R is literal string which allows u to write sql query on multiple lines without using escape sequence 
    // real means using decimal values
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
        //if create table command fails then undo the transaction
        return false;
    }

    db.commit(); // permenently commit the table 

    createDashboardTables();  // calling this function which tell compiler to create 5 more tables and then returns true 
    return true;
}

bool DatabaseManager::saveUserRegistration(const user& newUser) // taker user variable from user class referring to user.h
{
    if (!db.isOpen()) {
        QMessageBox::critical(nullptr, "Database Error", "The database is not open.");
        return false;
    } // safety check befote making any type of changes in database 

    QString hashedPassword = QString(QCryptographicHash::hash(
        newUser.getPassword().toUtf8(), QCryptographicHash::Sha256).toHex());
    /* 1. takes raw password whihc is readable 
    * 2. convert it into raw bytes UTF format bcz the hash function needs bytes not text
    * 3. run a SHA-256 algorithm and then save a encrypted result
    * 4. convert the binary result into hexa decimal format 
    */

    QSqlQuery query(db);

    query.prepare(R"(
        INSERT INTO User (username, password, name, email, phone, targetWeight, weight, height, gender, bloodGroup, dob, medicalHistory, consent) 
        VALUES (:username, :password, :name, :email, :phone, :targetWeight, :weight, :height, :gender, :bloodGroup, :dob, :medicalHistory, :consent)
    )");

    //here : placeholder is used which tell the database i will give u username and its detail soon but for know we call it username 

    // here newUser.gotUsername is combination of getter and variable which is taking the value from the c++ and save it in database
    query.bindValue(":username", newUser.getUsername());
    query.bindValue(":password", hashedPassword);   // ensuring that hashed password saves in table 
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
    query.bindValue(":consent", newUser.getConsent() ? 1 : 0);   // 1 for agreed and 0 for not 

    // SQL Error Trap to catch specific rejection reasons
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Registration Failed", "The database rejected the data. Reason:\n\n" + query.lastError().text());
        return false;
    }
	// .exec is actually executing the SQL query and if it fails it will show a message box with the specific error message from the database

    return true;
}

user DatabaseManager::getUserDetails(const QString& username)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM User WHERE username = :username");
    query.bindValue(":username", username);

    //.exec() runs the query .next() moves to the first result row If both succeed, theres a matching user in the database
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
	return user(); // if no user found return default user object with empty values
}

void DatabaseManager::createDashboardTables()
{
    db.transaction();
    QSqlQuery query(db);

    //these all tables depend on parent table [user] these all are child tables

    /*
 * WHY DAILYMETRICS TABLE?
 * This table acts as a summary (or cache) table to optimize query performance.
 * Instead of running heavy SQL calculations (SUM, AVG) on thousands of rows
 * inside the Activity, Vitals, and Nutrition logs to generate historical charts,
 * we store the pre-calculated daily totals here. This allows the Dashboard
 * to load instantly by reading just a few summarized rows.
 */



 /*
* Why we didnt add foreign key?
* 'username' acts as a conceptual foreign key to the 'User' table.
* Explicit SQL constraints are omitted because:
* 1. C++ session logic guarantees data integrity (only valid users can insert).
* 2. SQLite disables foreign key enforcement by default anyway.
*/
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
	DailyMetrics data = { 0 };  //c++ trick to meke new object named dailymetrics and initialize all its members to zero or default values
	QString dateStr = date.toString("yyyy-MM-dd");  // converting QDate to string format which is compatible with our database storage and query

    QSqlQuery nutritionQuery(db);

	//calculating the total calories, protein, carbs and fats consumed by the user on a specific date by summing up the values from the NutritionLog table for that user and date
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

   //repeating the same process as above
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

	//here desc limit 1 means if there are multiple entries for the same date we will take the latest one which is more relevant for the dashboard display and analysis 
    vitalsQuery.bindValue(":usr", username);
    vitalsQuery.bindValue(":date", dateStr);

    if (vitalsQuery.exec() && vitalsQuery.next()) {
        data.heartRate = vitalsQuery.value(0).toInt();
        double totalSleep = vitalsQuery.value(1).toDouble();
		// converting total sleep hours into hours and minutes format for better readability on the dashboard
        data.sleepHours = static_cast<int>(totalSleep);
        data.sleepMinutes = static_cast<int>((totalSleep - data.sleepHours) * 60);

        double recPct = (totalSleep / 8.0) * 100.0;
		// calculating recovery percentage based on sleep hours with a cap at 100% to avoid unrealistic values
        data.recoveryPercentage = (recPct > 100.0) ? 100 : static_cast<int>(recPct);
    }

    return data;
}


//shows it will return a activity record list then it shows that getActivityLog function is member of databasemanager class 
//giving address so that the functio dont make a copy of username and data in another memory and const used so it becomes read only memory
QList<ActivityRecord> DatabaseManager::getActivityLog(const QString& username, const QDate& date)
{
    QList<ActivityRecord> logList; //created empty list of activity record and this will hold multiple activity records for a specific user 
    QSqlQuery query(db);

    //extracting all activity records for a specific user and date from the ActivityLog table
    query.prepare("SELECT * FROM ActivityLog WHERE username = :username AND record_date = :date");
    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    

    if (query.exec()) {// checks wheather the query runs without error
        while (query.next()) {
            ActivityRecord record;   //creating record object agian and again
            record.exerciseName = query.value("exercise_name").toString();
            record.durationMins = query.value("duration_mins").toInt();
            record.intensity = query.value("intensity").toString();
            record.caloriesBurned = query.value("calories_burned").toInt();
            record.syncStatus = query.value("sync_status").toString();

            //when all exercise data complete in one pack it will save all the data in loglist
            logList.append(record);
        }
    }
    return logList; // then it will return the list which will show in table using ui 
}

//return true if the record is successfully saved in database otherwise false
bool DatabaseManager::saveActivityRecord(const QString& username, const QDate& date, const QString& exerciseName, int durationMins, int intensity, int caloriesBurned, int steps)
{
    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO ActivityLog (username, record_date, exercise_name, duration_mins, intensity, calories_burned, step_count, sync_status) 
        VALUES (:username, :date, :exercise, :duration, :intensity, :calories, :steps, 'PENDING')
    )");
    //initialize all data sync status as pending and when user click on sync button it will show synced 

    query.bindValue(":username", username);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":exercise", exerciseName);
    query.bindValue(":duration", durationMins);
    query.bindValue(":intensity", intensity);
    query.bindValue(":calories", caloriesBurned);
    query.bindValue(":steps", steps);

    return query.exec();    //perform insertion operation and return true if successful otherwise false
}

bool DatabaseManager::saveVitalsRecord(const QString& username, const QDate& date, double weight, double bodyTemp, int heartRate, int bpSys, int bpDia, int bloodSugar, int stressLevel, double sleepHours)
{

    // This ensures the user's main profile always shows their latest weight, instead of the old weight from the day they created the account.

    QSqlQuery weightQuery(db);
    weightQuery.prepare("UPDATE User SET weight = :weight WHERE username = :username");
    weightQuery.bindValue(":weight", weight);
    weightQuery.bindValue(":username", username);
    weightQuery.exec();
    

    //now update the vitals record
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

	//here we used columns names with select query but we can also use * but using column names is better for readability and maintainability of code and also it is more efficient as it only fetches the required data instead of fetching all columns which may include unnecessary data

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
    // this function updates the sync status of all activities 
    query.prepare("UPDATE ActivityLog SET sync_status = 'SYNCED' WHERE username = :usr AND sync_status = 'PENDING'");
    query.bindValue(":usr", username);

    return query.exec();
}