#include "History.h"
#include "ui_History.h"
#include <QDebug>
#include <QDate>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>


HistoryScreen::HistoryScreen(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::HistoryScreen),
    currentPage(1),
    recordsPerPage(6),
    totalRecords(0)
{
    ui->setupUi(this);
    this->showMaximized();
    this->setWindowTitle("ZenithHealthSuite - History");

    ui->comboFilter->clear();
    ui->comboFilter->addItems({ "Activity Logs", "Vitals Logs", "Nutrition Logs" });

    loadTopCards();
    on_comboFilter_currentIndexChanged("Activity Logs");
}

HistoryScreen::~HistoryScreen()
{
    delete ui;
}

void HistoryScreen::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen();
    dashboard->show();
    this->close();
}


void HistoryScreen::on_analyticsButton_clicked() {
    qDebug() << "Analytics coming soon...";
}

void HistoryScreen::on_activityButton_clicked() {
    Activity* activityScreen = new Activity();
    activityScreen->show();
    this->close();
}

void HistoryScreen::on_goalsButton_clicked() {
    qDebug() << "Goals coming soon...";
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



void HistoryScreen::setCurrentUser(const QString& username)
{
    currentUser = username;
    loadTopCards();
    loadTableData();
}

void HistoryScreen::loadTopCards()
{
    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM ActivityLog WHERE username = :user");
    query.bindValue(":user", currentUser);
    if (query.exec() && query.next()) {
        ui->lblTotalWorkoutsValue->setText(query.value(0).toString());
    }

    query.prepare("SELECT SUM(duration_mins) FROM ActivityLog WHERE username = :user");
    query.bindValue(":user", currentUser);
    if (query.exec() && query.next()) {
        int totalMins = query.value(0).toInt();
        ui->lblActiveHoursValue->setText(QString::number(totalMins / 60) + "h");
    }

    query.prepare("SELECT AVG(heart_rate) FROM VitalsLog WHERE username = :user AND heart_rate > 0");
    query.bindValue(":user", currentUser);
    if (query.exec() && query.next()) {
        ui->lblAvgHeartRateValue->setText(QString::number(query.value(0).toInt()) + " bpm");
    }

    query.prepare("SELECT COUNT(DISTINCT record_date) FROM ActivityLog WHERE username = :user");
    query.bindValue(":user", currentUser);
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

void HistoryScreen::on_comboFilter_currentIndexChanged(const QString& filter)
{
    currentPage = 1; 
    setupTableColumns(filter);
    loadTableData();
}

void HistoryScreen::loadTableData()
{
    QString filter = ui->comboFilter->currentText();
    QString tableName;

    if (filter == "Activity Logs") tableName = "ActivityLog";
    else if (filter == "Vitals Logs") tableName = "VitalsLog";
    else if (filter == "Nutrition Logs") tableName = "NutritionLog";

    QSqlQuery query;

    query.prepare("SELECT COUNT(*) FROM " + tableName + " WHERE username = :user");
    query.bindValue(":user", currentUser);
    if (query.exec() && query.next()) {
        totalRecords = query.value(0).toInt();
    }

    int offset = (currentPage - 1) * recordsPerPage;
    query.prepare("SELECT * FROM " + tableName + " WHERE username = :user ORDER BY record_date DESC LIMIT :limit OFFSET :offset");
    query.bindValue(":user", currentUser);
    query.bindValue(":limit", recordsPerPage);
    query.bindValue(":offset", offset);

    if (query.exec()) {
        ui->tableHistory->setRowCount(0); 
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
            row++;
        }
    }
    updatePaginationInfo();
}

void HistoryScreen::updatePaginationInfo()
{
    int startRecord = (currentPage - 1) * recordsPerPage + 1;
    int endRecord = qMin(currentPage * recordsPerPage, totalRecords);

    if (totalRecords == 0) {
        ui->lblShowingRecords->setText("No records found.");
    }
    else {
        ui->lblShowingRecords->setText(QString("Showing %1-%2 of %3 records").arg(startRecord).arg(endRecord).arg(totalRecords));
    }

    ui->btnPage1->setText(QString::number(currentPage));
}

void HistoryScreen::on_btnPrevPage_clicked()
{
    if (currentPage > 1) {
        currentPage--;
        loadTableData();
    }
}

void HistoryScreen::on_btnNextPage_clicked()
{
    int totalPages = (totalRecords + recordsPerPage - 1) / recordsPerPage;
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

    QMessageBox::information(this, "Success", "History successfully exported to PDF!");
}