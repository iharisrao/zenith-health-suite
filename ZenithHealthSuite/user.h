#ifndef user_h          //it acts as bodyguard as prevents the computer from accidentally reading this blueprint twice and crashing the app
#define user_h

// we use <qtstring> instead of <string> because we are using Qt framework for our application and it provides its own string class which is more suitable for our needs
#include <QString>

class user {
private:
	//All data is private so it cannot be accidentally modified or deleted by other parts of the application.
	Qstring username;
	Qstring password;
	Qstring name;
	Qstring gender;
	int age;
	Qstring email;
	Qstring contactNumber;
	double weight;
	double height;
	Qtring medicalHistory;

public:
	user(Qstring usr, Qstring pass, Qstring n, Qstring g, int a, Qstring e, Qstring c, double w, double h, Qstring med);
	
	Qstring getUsername() const;
	Qstring getPassword() const;
	Qstring getName() const;
	Qstring getGender() const;
	int getAge() const;
	Qstring getEmail() const;
	Qstring getContactNumber() const;
	double getWeight() const;
	double getHeight() const;
	Qstring getMedicalHistory() const;

	double calculateTargetBPM() const;
};
// end of include guard
#endif 
