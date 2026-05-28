#ifndef HISTORY_H
#define HISTORY_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include "DashboardScreen.h"
#include "Activity.h"
#include "analytics.h"
#include "Goals.h"
#include "DatabaseManager.h" 
#include "user.h" 

namespace Ui {
    class HistoryScreen;
}

class HistoryScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit HistoryScreen(const user& currentUserObj, QWidget* parent = nullptr);
    ~HistoryScreen();

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

    void handleFilterChange(const QString& filter);

    void on_btnPrevPage_clicked();
    void on_btnNextPage_clicked();
    void on_btnPage1_clicked();
    void on_btnPage2_clicked();
    void on_btnPage3_clicked();

    void on_btnExport_clicked();
    void on_syncDataButton_clicked();

private:
    Ui::HistoryScreen* ui;
    DatabaseManager dbManager;
    user loggedInUser;
    int currentPage;
    int recordsPerPage;
    int totalRecords;
    int totalPages;

    void loadTopCards();
    void setupTableColumns(const QString& filter);
    void loadTableData();
    void updatePaginationInfo();
    void updatePaginationButtonsUI();
};

#endif