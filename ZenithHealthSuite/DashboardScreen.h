// guard which ensures that header file in only read once
#ifndef DASHBOARDSCREEN_H
#define DASHBOARDSCREEN_H

#include <QMainWindow>  //library which should be include as all main screen are made on this QT library        
#include <QDebug>         //used to check wheather all apps runs perfectly and if not will show an error 
#include <QTimer>       //used to add timer feature in our app
#include <QMessageBox> 
#include "Activity.h"     //as to run dashboard screen it needs all these other screens 
#include "History.h"
#include "analytics.h"
#include "Goals.h"
#include "DatabaseManager.h"
#include "user.h" 


//when user make ui using drag and drop the QT writes its code automatically here we are telling the compiler to not to mix dashboard screen c++ with its ui
//namespace acts as a folder so the files donot mix 
QT_BEGIN_NAMESPACE
namespace Ui { class DashboardScreen; }
QT_END_NAMESPACE

//here QMainWindow is used which tells the user that Dashboard Screen is not a simple screen it is QT mai window 
class DashboardScreen : public QMainWindow
{
    Q_OBJECT  //magical button which tells qt to that tis screen will work on signal and slots.if we dont use it our buttons will not work

public:
    //explicit is strict function which tells compiler to not to make changes in code on its user wait for user permission
    
    //all features are child on someone else here QWidget and nullptr ensures that main dashboard screen is independent  and assume it as nullptr
    explicit DashboardScreen(const user& currentUserObj, QWidget* parent = nullptr);
    ~DashboardScreen();
    //if changes made on activity screen it will also shown on dashboard scren using this function
    void updateDashboardMetrics(int caloriesConsumed, int caloriesGoal, int steps, int activeEnergy);



private slots:

    //when the user presses any button it makes an signal and to catch that signal we make a slot function
    void on_dashboardButton_clicked();
    void on_analyticsButton_clicked();
    void on_activityButton_clicked();
    void on_goalsButton_clicked();
    void on_historyButton_clicked();
    void on_settingsButton_clicked();
    void on_supportButton_clicked();
    void on_profileButton_clicked();
    void on_bellButton_clicked();
    
    void on_syncDataButton_clicked();

private:
    //Encapsulation
    //ui is pointing to the drag and drop pointer 
    Ui::DashboardScreen* ui;
    //dbManager is used to communicate to local database 
    DatabaseManager dbManager;
    user loggedInUser;

    void setupHeartRateChart();
    void setupActiveEnergyChart();
    void loadRealData();
};

#endif