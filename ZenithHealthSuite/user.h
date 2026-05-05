#ifndef user_h          //it acts as bodyguard as prevents the computer from accidentally reading this blueprint twice and crashing the app
#define user_h

// we use <qtstring> instead of <string> because we are using Qt framework for our application and it provides its own string class which is more suitable for our needs
#include <QString>

class user {
private:
	//All data is private so it cannot be accidentally modified or deleted by other parts of the application.
	QString username;
	QString password;
	QString name;
	QString	gender;
	int age;
	QString email;
	QString contactNumber;
	double weight;
	double height;
	QString medicalHistory;
public:
	user(QString usr, QString pass, QString n, QString g, int a, QString e, QString c, double w, double h, QString med);
	
	QString getUsername() const;
	QString getPassword() const;
	QString getName() const;
	QString getGender() const;
	int getAge() const;
	QString getEmail() const;
	QString getContactNumber() const;
	double getWeight() const;
	double getHeight() const;
	QString getMedicalHistory() const;

	double calculateTargetBPM() const;
};
// end of include guard
#endif 
