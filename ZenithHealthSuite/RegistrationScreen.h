#pragma once                                 //act like bouncers they stop C++ from accidentally reading this blueprint twice and crashing our app.
#ifndef RegistrationScreen_h
#define RegistrationScreen_h

#include<QMainWindow>
namespace Ui {                              // telling c++ that there is registration.ui is available so dont worry about it
	class RegistrationScreenClass;
}
class RegistrationScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistrationScreen(QWidget* parent = nullptr);
    ~RegistrationScreen();

    
private slots:
    void on_completeRegistrationButton_clicked();

private:
    Ui::RegistrationScreenClass* ui;
};

#endif         // RegistrationScreen_h