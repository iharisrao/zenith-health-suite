#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTableWidgetItem>

#include "DashboardScreen.h"
#include "Activity.h"
#include "History.h"
#include"Goals.h"

namespace Ui {
    class analyticsClass;
}

class analytics : public QMainWindow
{
    Q_OBJECT

public:
    explicit analytics(QWidget* parent = nullptr);
    ~analytics();

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
    Ui::analyticsClass* ui;

    void setupHeartRateChart();
    void setupSleepChart();
    void setupBarChart();
    void populateTable();
};

#endif