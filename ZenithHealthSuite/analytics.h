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

namespace Ui {
    class analyticsClass;
}

class analytics : public QMainWindow
{
    Q_OBJECT

public:
    explicit analytics(QWidget* parent = nullptr);
    ~analytics();

private:
    Ui::analyticsClass* ui;

    void setupHeartRateChart();
    void setupSleepChart();
    void setupBarChart();
    void populateTable();
};

#endif