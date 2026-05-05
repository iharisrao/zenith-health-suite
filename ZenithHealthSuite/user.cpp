#include "user.h"

//parameterized constructor
user::user(Qstring usr, Qstring pass, Qstring n, Qstring g, int a, Qstring e, Qstring c, double w, double h, Qstring med) {
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
Qstring user::getUsername() const {
		return username;
}
Qstring user::getPassword() const {
		return password;
}
Qstring user::getName() const {
		return name;
}
Qstring user::getGender() const {
	return gender;
}
int user::getAge() const {
	return age;
}
Qstring user::getEmail() const {
	return email;
}
Qstring user::getContactNumber() const {
	return contactNumber;
}
double user::getWeight() const {
	return weight;
}
double user::getHeight() const {
	return height;
}
Qstring user::getMedicalHistory() const {
	return medicalHistory;
}

// calculating BPM using Haskell & Fox formula
double user::calculateTargetBPM() const {
	double maxHeartRate = 220.0 - age;
	return maxHeartRate * 0.75;
}