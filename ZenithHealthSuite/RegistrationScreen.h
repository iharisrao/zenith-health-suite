#pragma once                                 //act like bouncers they stop C++ from accidentally reading this blueprint twice and crashing our app.
#ifndef RegistrationScreen_h
#define RegistrationScreen_h

#include<QMainWindow>
namespace Ui {                              // telling c++ that there is registration.ui is available so dont worry about it
	class RegistrationScreenClass;
}
class RegistrationScreen : public QMainWindow    //telling program that our app gets all superpower of QWidgets like minimizing etc
{
	Q_OBJECT        //The magic Qt radio antenna! We MUST have this line, otherwise our button clicks will never reach our C++ code.
public:
	explicit RegistrationScreen(QWidget* parent = nullptr);     //runs when we first open the registration screen

	~RegistrationScreen();     // cleans up memeory when we are done with the registration screen

private slots:
	void on_completeRegistrationButton_clicked();            //button which waits tills the user clicks it

private:

	// whenever we want to read what someone typed into the name or age boxes we have to use this 'ui' pointer to grab it
	Ui::RegistrationScreenClass* ui;
};

#endif         // RegistrationScreen_h