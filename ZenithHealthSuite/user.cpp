#include "user.h"

user::user() {
    weight = 0.0;
    height = 0.0;
    targetWeight = 0.0;
    consent = false;
}

user::user(
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
) {
    username = usr;
    password = pass;
    name = n;
    email = e;
    phone = p;
    targetWeight = tWeight;
    weight = w;
    height = h;
    gender = g;
    bloodGroup = bg;
    dob = d;
    medicalHistory = med;
    consent = c;
}

QString user::getUsername() const { 
    return username; 
}
QString user::getPassword() const { 
    return password; 
}
QString user::getName() const { 
    return name; 
}
QString user::getEmail() const { 
    return email; 
}
QString user::getPhone() const { 
    return phone; 
}
double user::getTargetWeight() const { 
    return targetWeight; 
}
double user::getWeight() const { 
    return weight; 
}
double user::getHeight() const { 
    return height; 
}
QString user::getGender() const { 
    return gender; 
}
QString user::getBloodGroup() const { 
    return bloodGroup; 
}
QString user::getDob() const { 
    return dob; 
}
QString user::getMedicalHistory() const { 
    return medicalHistory; 
}
bool user::getConsent() const { 
    return consent; 
}


void user::setWeight(double newWeight) { weight = newWeight; }
void user::setTargetWeight(double newTarget) { targetWeight = newTarget; }

double user::calculateTargetBPM(int age) const {
    double maxHeartRate = 220.0 - age;
    return maxHeartRate * 0.75;
}