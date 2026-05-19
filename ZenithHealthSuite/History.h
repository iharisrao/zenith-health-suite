#ifndef HISTORY_H
#define HISTORY_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include "DashboardScreen.h"
#include "Activity.h"


namespace Ui {
    class HistoryScreen;
}

class HistoryScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit HistoryScreen(QWidget* parent = nullptr);
    ~HistoryScreen();

    void setCurrentUser(const QString& username);

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


    void on_comboFilter_currentIndexChanged(const QString& filter);
    void on_btnPrevPage_clicked();
    void on_btnNextPage_clicked();
    void on_btnExport_clicked();

private:
    Ui::HistoryScreen* ui;
    QString currentUser;
    int currentPage;
    int recordsPerPage;
    int totalRecords;

    void loadTopCards();
    void setupTableColumns(const QString& filter);
    void loadTableData();
    void updatePaginationInfo();
};

#endif 