#ifndef DASHBOARDSCREEN_H
#define DASHBOARDSCREEN_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>       
#include <QMessageBox> 
#include "Activity.h"
#include "History.h"
#include "analytics.h"
#include "Goals.h"
#include "DatabaseManager.h"
#include "user.h" 

QT_BEGIN_NAMESPACE
namespace Ui { class DashboardScreen; }
QT_END_NAMESPACE

class DashboardScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardScreen(const user& currentUserObj, QWidget* parent = nullptr);
    ~DashboardScreen();

    void updateDashboardMetrics(int caloriesConsumed, int caloriesGoal, int steps, int activeEnergy);



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

    void on_syncDataButton_clicked();

private:
    Ui::DashboardScreen* ui;
    DatabaseManager dbManager;
    user loggedInUser;

    void setupHeartRateChart();
    void setupActiveEnergyChart();
    void loadRealData();
};

#endif