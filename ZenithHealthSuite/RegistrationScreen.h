#pragma once                                 //act like bouncers they stop C++ from accidentally reading this blueprint twice and crashing our app.
#ifndef RegistrationScreen_h
#define RegistrationScreen_h

#include <QMainWindow>

namespace Ui {                              
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
    void on_btnBackToLogin_clicked();

private:
    Ui::RegistrationScreenClass* ui;
};

#endif         