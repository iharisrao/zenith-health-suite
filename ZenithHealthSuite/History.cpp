#include "History.h"
#include "ui_History.h"
#include <QDebug>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QTimer>

HistoryScreen::HistoryScreen(const user& currentUserObj, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::HistoryScreen),
    loggedInUser(currentUserObj),
    currentPage(1),
    recordsPerPage(6),
    totalRecords(0),
    totalPages(1)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("ZenithHealthSuite - History [" + loggedInUser.getName() + "]");

    ui->comboFilter->clear();
    ui->comboFilter->addItems({ "Activity Logs", "Vitals Logs", "Nutrition Logs" });

    connect(ui->comboFilter, &QComboBox::currentTextChanged, this, &HistoryScreen::handleFilterChange);

    loadTopCards();
    handleFilterChange("Activity Logs");
}

HistoryScreen::~HistoryScreen() { delete ui; }

void HistoryScreen::on_syncDataButton_clicked()
{
    ui->syncDataButton->setText("Syncing...");
    ui->syncDataButton->setEnabled(false);

    QTimer::singleShot(1500, this, [=]() {
        loadTopCards();
        loadTableData();

        ui->syncDataButton->setText("Sync Data");
        ui->syncDataButton->setEnabled(true);

        QMessageBox::information(this, "Sync Complete", "All history data synchronized!");
        });
}

void HistoryScreen::loadTopCards()
{
    QSqlQuery query;
    QString usr = loggedInUser.getUsername();

    query.prepare("SELECT COUNT(*) FROM ActivityLog WHERE username = :user");
    query.bindValue(":user", usr);
    if (query.exec() && query.next()) ui->lblTotalWorkoutsValue->setText(query.value(0).toString());

    query.prepare("SELECT SUM(duration_mins) FROM ActivityLog WHERE username = :user");
    query.bindValue(":user", usr);
    if (query.exec() && query.next()) {
        int totalMins = query.value(0).toInt();
        int hours = totalMins / 60;
        int mins = totalMins % 60;
        ui->lblActiveHoursValue->setText(QString("%1h %2m").arg(hours).arg(mins));
    }

    query.prepare("SELECT AVG(heart_rate) FROM VitalsLog WHERE username = :user AND heart_rate > 0");
    query.bindValue(":user", usr);
    if (query.exec() && query.next()) {
        int avgHR = query.value(0).toInt();
        ui->lblAvgHeartRateValue->setText(avgHR > 0 ? QString::number(avgHR) + " bpm" : "0 bpm");
    }

    query.prepare("SELECT COUNT(DISTINCT record_date) FROM ActivityLog WHERE username = :user AND record_date >= date('now', '-30 days')");
    query.bindValue(":user", usr);
    if (query.exec() && query.next()) {
        int daysActive = query.value(0).toInt();
        int score = (daysActive * 100) / 30;
        if (score > 100) score = 100;
        ui->lblConsistencyValue->setText(QString::number(score) + "%");
    }
}

void HistoryScreen::setupTableColumns(const QString& filter)
{
    ui->tableHistory->clear();
    ui->tableHistory->setRowCount(0);

    if (filter == "Activity Logs") {
        ui->tableHistory->setColumnCount(5);
        ui->tableHistory->setHorizontalHeaderLabels({ "DATE", "EXERCISE", "DURATION", "INTENSITY", "CALORIES" });
    }
    else if (filter == "Vitals Logs") {
        ui->tableHistory->setColumnCount(5);
        ui->tableHistory->setHorizontalHeaderLabels({ "DATE", "WEIGHT", "BP (SYS/DIA)", "HEART RATE", "SUGAR" });
    }
    else if (filter == "Nutrition Logs") {
        ui->tableHistory->setColumnCount(5);
        ui->tableHistory->setHorizontalHeaderLabels({ "DATE", "MEAL", "FOOD", "MACROS (P/C/F)", "CALORIES" });
    }
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void HistoryScreen::handleFilterChange(const QString& filter)
{
    currentPage = 1;
    setupTableColumns(filter);
    loadTableData();
}

void HistoryScreen::loadTableData()
{
    ui->tableHistory->setRowCount(0);

    QString filter = ui->comboFilter->currentText();
    QString tableName;

    if (filter == "Activity Logs") tableName = "ActivityLog";
    else if (filter == "Vitals Logs") tableName = "VitalsLog";
    else if (filter == "Nutrition Logs") tableName = "NutritionLog";

    if (tableName.isEmpty()) return;

    QSqlQuery query;
    QString usr = loggedInUser.getUsername();

    query.prepare("SELECT COUNT(*) FROM " + tableName + " WHERE username = :user");
    query.bindValue(":user", usr);
    if (query.exec() && query.next()) {
        totalRecords = query.value(0).toInt();
    }

    int offset = (currentPage - 1) * recordsPerPage;
    query.prepare("SELECT * FROM " + tableName + " WHERE username = :user ORDER BY record_date DESC LIMIT :limit OFFSET :offset");
    query.bindValue(":user", usr);
    query.bindValue(":limit", recordsPerPage);
    query.bindValue(":offset", offset);

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            ui->tableHistory->insertRow(row);
            ui->tableHistory->setItem(row, 0, new QTableWidgetItem(query.value("record_date").toString()));

            if (filter == "Activity Logs") {
                ui->tableHistory->setItem(row, 1, new QTableWidgetItem(query.value("exercise_name").toString()));
                ui->tableHistory->setItem(row, 2, new QTableWidgetItem(query.value("duration_mins").toString() + " Min"));
                ui->tableHistory->setItem(row, 3, new QTableWidgetItem(query.value("intensity").toString() + "/10"));
                ui->tableHistory->setItem(row, 4, new QTableWidgetItem(query.value("calories_burned").toString() + " kcal"));
            }
            else if (filter == "Vitals Logs") {
                ui->tableHistory->setItem(row, 1, new QTableWidgetItem(query.value("weight").toString() + " kg"));
                QString bp = query.value("bp_sys").toString() + "/" + query.value("bp_dia").toString();
                ui->tableHistory->setItem(row, 2, new QTableWidgetItem(bp));
                ui->tableHistory->setItem(row, 3, new QTableWidgetItem(query.value("heart_rate").toString() + " bpm"));
                ui->tableHistory->setItem(row, 4, new QTableWidgetItem(query.value("blood_sugar").toString() + " mg/dL"));
            }
            else if (filter == "Nutrition Logs") {
                ui->tableHistory->setItem(row, 1, new QTableWidgetItem(query.value("meal_type").toString()));
                ui->tableHistory->setItem(row, 2, new QTableWidgetItem(query.value("food_name").toString()));
                QString macros = query.value("protein_g").toString() + "p / " + query.value("carbs_g").toString() + "c / " + query.value("fats_g").toString() + "f";
                ui->tableHistory->setItem(row, 3, new QTableWidgetItem(macros));
                ui->tableHistory->setItem(row, 4, new QTableWidgetItem(query.value("calories").toString() + " kcal"));
            }

            for (int col = 0; col < ui->tableHistory->columnCount(); ++col) {
                if (ui->tableHistory->item(row, col)) {
                    ui->tableHistory->item(row, col)->setTextAlignment(Qt::AlignCenter);
                }
            }
            row++;
        }
    }
    updatePaginationInfo();
}

void HistoryScreen::updatePaginationInfo()
{
    int startRecord = totalRecords == 0 ? 0 : ((currentPage - 1) * recordsPerPage) + 1;
    int endRecord = qMin(currentPage * recordsPerPage, totalRecords);

    totalPages = (totalRecords + recordsPerPage - 1) / recordsPerPage;
    if (totalPages == 0) totalPages = 1;

    if (totalRecords == 0) {
        ui->lblShowingRecords->setText("No records found in " + ui->comboFilter->currentText());
    }
    else {
        ui->lblShowingRecords->setText(QString("Showing %1-%2 of %3 records").arg(startRecord).arg(endRecord).arg(totalRecords));
    }

    updatePaginationButtonsUI();
}

void HistoryScreen::updatePaginationButtonsUI()
{
    int btn1Val = currentPage;
    int btn2Val = currentPage + 1;
    int btn3Val = currentPage + 2;

    if (currentPage == totalPages && totalPages >= 3) {
        btn1Val = totalPages - 2; btn2Val = totalPages - 1; btn3Val = totalPages;
    }
    else if (currentPage == totalPages - 1 && totalPages >= 3) {
        btn1Val = totalPages - 2; btn2Val = totalPages - 1; btn3Val = totalPages;
    }

    ui->btnPage1->setText(QString::number(btn1Val));
    ui->btnPage2->setText(QString::number(btn2Val));
    ui->btnPage3->setText(QString::number(btn3Val));

    ui->btnPage1->setVisible(true);
    ui->btnPage2->setVisible(true);
    ui->btnPage3->setVisible(true);

    ui->btnPage1->setEnabled(true);
    ui->btnPage2->setEnabled(btn2Val <= totalPages);
    ui->btnPage3->setEnabled(btn3Val <= totalPages);

    QString baseStyle = R"(
        QPushButton {
            min-width: 35px;
            max-width: 35px;
            min-height: 35px;
            max-height: 35px;
            border-radius: 6px;
            font-weight: bold;
            font-size: 14px;
        }
    )";

    QString activeStyle = baseStyle + R"(
        QPushButton { 
            background-color: #1558A8; 
            color: white; 
            border: 1px solid #1558A8;
        }
    )";

    QString inactiveStyle = baseStyle + R"(
        QPushButton { 
            background-color: transparent; 
            color: #1558A8; 
            border: 1px solid #E2E8F0; 
        }
        QPushButton:hover { 
            background-color: #EFF6FF; 
            border: 1px solid #1558A8;
        }
    )";

    QString disabledStyle = baseStyle + R"(
        QPushButton { 
            background-color: transparent; 
            color: #CBD5E1; 
            border: 1px solid #E2E8F0; 
        }
    )";

    ui->btnPage1->setStyleSheet(btn1Val == currentPage ? activeStyle : inactiveStyle);
    ui->btnPage2->setStyleSheet(btn2Val <= totalPages ? (btn2Val == currentPage ? activeStyle : inactiveStyle) : disabledStyle);
    ui->btnPage3->setStyleSheet(btn3Val <= totalPages ? (btn3Val == currentPage ? activeStyle : inactiveStyle) : disabledStyle);
}

void HistoryScreen::on_btnPage1_clicked() { currentPage = ui->btnPage1->text().toInt(); loadTableData(); }
void HistoryScreen::on_btnPage2_clicked() { currentPage = ui->btnPage2->text().toInt(); loadTableData(); }
void HistoryScreen::on_btnPage3_clicked() { currentPage = ui->btnPage3->text().toInt(); loadTableData(); }

void HistoryScreen::on_btnPrevPage_clicked()
{
    if (currentPage > 1) {
        currentPage--;
        loadTableData();
    }
}
void HistoryScreen::on_btnNextPage_clicked()
{
    if (currentPage < totalPages) {
        currentPage++;
        loadTableData();
    }
}

void HistoryScreen::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF", "Health_History_Report.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    QString html = "<h2>Zenith Health Suite - " + ui->comboFilter->currentText() + "</h2>";
    html += "<table border='1' cellspacing='0' cellpadding='5' width='100%'><tr>";

    for (int c = 0; c < ui->tableHistory->columnCount(); ++c) {
        html += "<th bgcolor='#F8FAFC'>" + ui->tableHistory->horizontalHeaderItem(c)->text() + "</th>";
    }
    html += "</tr>";

    for (int r = 0; r < ui->tableHistory->rowCount(); ++r) {
        html += "<tr>";
        for (int c = 0; c < ui->tableHistory->columnCount(); ++c) {
            html += "<td>" + ui->tableHistory->item(r, c)->text() + "</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Done", "History successfully exported to PDF!");
}

void HistoryScreen::on_dashboardButton_clicked() { 
    DashboardScreen* dashboard = new DashboardScreen(loggedInUser); 
    dashboard->show(); 
    this->close(); 
}
void HistoryScreen::on_analyticsButton_clicked() { 
    analytics* analyticsScreen = new analytics(loggedInUser); 
    analyticsScreen->show(); 
    this->close(); 
}
void HistoryScreen::on_activityButton_clicked() { 
    Activity* activityScreen = new Activity(loggedInUser); 
    activityScreen->show(); 
    this->close(); 
}
void HistoryScreen::on_goalsButton_clicked() { 
    Goals* goalsscreen = new Goals(loggedInUser); 
    goalsscreen->show(); 
    this->close(); 
}
void HistoryScreen::on_historyButton_clicked() { 
    qDebug() << "Already on the History!"; 
}
void HistoryScreen::on_settingsButton_clicked() { 
    qDebug() << "Settings coming soon..."; 
}
void HistoryScreen::on_supportButton_clicked() { 
    qDebug() << "Support coming soon..."; 
}
void HistoryScreen::on_profileButton_clicked() { 
    qDebug() << "User profile..."; 
}
void HistoryScreen::on_bellButton_clicked() { 
    qDebug() << "No new notifications."; 
}