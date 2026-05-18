#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <QMainWindow> 
#include <QDate>
#include <QDebug>
#include "DashboardScreen.h"

namespace Ui {
    class Activity;
}

class Activity : public QMainWindow
{
    Q_OBJECT

public:
    explicit Activity(QWidget* parent = nullptr);
    ~Activity();

private slots:
 
    void on_dashboardButton_clicked();
    void on_analyticsButton_clicked();
    void on_activityButton_clicked();
    void on_goalsButton_clicked();
    void on_historyButton_clicked();
    void on_settingsButton_clicked();
    void on_supportButton_clicked();


    void on_profileButton_clicked();
    void on_bellButton_clicked();


    void on_btnSaveActivity_clicked();   
    void on_btnSaveVitals_clicked();  
    void on_btnSaveMeal_clicked();    

private:
    Ui::Activity* ui;

    void setupCurrentDate();
    void loadIntegratedMetrics();
};

#endif 