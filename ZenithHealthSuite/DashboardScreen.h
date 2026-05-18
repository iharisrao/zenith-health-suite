#ifndef DASHBOARDSCREEN_H
#define DASHBOARDSCREEN_H

#include <QMainWindow>
#include <QDebug> 
#include "Activity.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DashboardScreen; }
QT_END_NAMESPACE

class DashboardScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardScreen(QWidget* parent = nullptr);
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

private:
	Ui::DashboardScreen *ui;
    void setupHeartRateChart();
    void setupActiveEnergyChart();
    void setupTableBadges();
};

#endif // DASHBOARDSCREEN_H