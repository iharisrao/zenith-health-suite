#include "user.h"

//parameterized constructor
user::user(QString usr, QString pass, QString n, QString g, int a, QString e, QString c, double w, double h, QString																																																																				 med) {
	username = usr;
	password = pass;
	name = n;
	gender = g;
	age = a;
	email = e;
	contactNumber = c;
	weight = w;
	height = h;
	medicalHistory = med;
}
// calling getter
QString user::getUsername() const {
		return username;
}
QString user::getPassword() const {
		return password;
}
QString user::getName() const {
		return name;
}
QString user::getGender() const {
	return gender;
}
int user::getAge() const {
	return age;
}
QString user::getEmail() const {
	return email;
}
QString user::getContactNumber() const {
	return contactNumber;
}
double user::getWeight() const {
	return weight;
}
double user::getHeight() const {
	return height;
}
QString user::getMedicalHistory() const {
	return medicalHistory;
}

// calculating BPM using Haskell & Fox formula
double user::calculateTargetBPM() const {
	double maxHeartRate = 220.0 - age;
	return maxHeartRate * 0.75;
}