#ifndef USER_H
#define USER_H

#include <QString>

class user {
private:
    QString username;
    QString password;
    QString name;
    QString email;
    QString phone;
    double targetWeight;
    double weight;
    double height;
    QString gender;
    QString bloodGroup;
    QString dob;
    QString medicalHistory;
    bool consent;

public:

    user();

    user(
        QString usr,
        QString pass,
        QString n,
        QString e,
        QString p,
        double tWeight,
        double w,
        double h,
        QString g,
        QString bg,
        QString d,
        QString med,
        bool c
    );

    QString getUsername() const;
    QString getPassword() const;
    QString getName() const;
    QString getEmail() const;
    QString getPhone() const;
    double getTargetWeight() const;
    double getWeight() const;
    double getHeight() const;
    QString getGender() const;
    QString getBloodGroup() const;
    QString getDob() const;
    QString getMedicalHistory() const;
    bool getConsent() const;

    void setWeight(double newWeight);
    void setTargetWeight(double newTarget);

    double calculateTargetBPM(int age) const;
};

#endif