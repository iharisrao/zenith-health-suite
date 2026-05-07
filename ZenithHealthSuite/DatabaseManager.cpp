#include "DatabaseManager.h"
#include <QDebug>
#include <QCryptographicHash>

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