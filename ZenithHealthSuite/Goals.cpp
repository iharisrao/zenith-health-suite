#include "Goals.h"
#include "GoalsCard.h"
#include <QMessageBox>
#include <QTimer> 
#include <QDebug>
#include <QDate>      
#include <QSqlQuery>  

Goals::Goals(const user& currentUserObj, QWidget* parent)
    : QMainWindow(parent), loggedInUser(currentUserObj)
{
    ui.setupUi(this);
    this->showMaximized();

    this->setWindowTitle("ZenithHealthSuite - Goals [" + loggedInUser.getName() + "]");

    ui.comboCategory->clear();
    ui.comboCategory->addItem("Biometric Data");
    ui.comboCategory->addItem("Physical Activity");
    ui.comboCategory->addItem("Nutritional Target");
    ui.comboCategory->addItem("Mental Health");
    ui.comboCategory->addItem("Recovery");

    connect(ui.btnInitializeTarget, &QPushButton::clicked, this, &Goals::onInitializeTargetClicked);

    connect(ui.btnDaily, &QPushButton::clicked, this, [=]() { setFrequency("Daily"); });
    connect(ui.btnWeekly, &QPushButton::clicked, this, [=]() { setFrequency("Weekly"); });
    connect(ui.btnQuarterly, &QPushButton::clicked, this, [=]() { setFrequency("Quarterly"); });

    setFrequency("Daily");
    setupScrollArea();
    loadGoalsFromDatabase();
}

Goals::~Goals()
{
}

void Goals::on_syncDataButton_clicked()
{
    ui.syncDataButton->setText("Syncing...");
    ui.syncDataButton->setEnabled(false);

    QTimer::singleShot(1500, this, [=]() {
        loadGoalsFromDatabase();

        ui.syncDataButton->setText("Sync Data");
        ui.syncDataButton->setEnabled(true);

        QMessageBox::information(this, "Sync Complete", "All goals and targets successfully synchronized!");
        });
}

void Goals::setFrequency(QString freq)
{
    currentFrequency = freq;

    QString activeStyle = "background-color: #1558A8; color: white; border-radius: 4px; font-weight: bold;";
    QString inactiveStyle = "background-color: transparent; color: #1558A8; border: 1px solid #E2E8F0; border-radius: 4px; font-weight: bold;";

    ui.btnDaily->setStyleSheet(freq == "Daily" ? activeStyle : inactiveStyle);
    ui.btnWeekly->setStyleSheet(freq == "Weekly" ? activeStyle : inactiveStyle);
    ui.btnQuarterly->setStyleSheet(freq == "Quarterly" ? activeStyle : inactiveStyle);
}

QString Goals::getSelectedFrequency()
{
    return currentFrequency;
}

void Goals::on_dashboardButton_clicked() {
    DashboardScreen* dashboard = new DashboardScreen(loggedInUser);
    dashboard->show();
    this->close();
}
void Goals::on_analyticsButton_clicked() {
    analytics* analyticsScreen = new analytics(loggedInUser);
    analyticsScreen->show();
    this->close();
}
void Goals::on_activityButton_clicked() {
    Activity* activityScreen = new Activity(loggedInUser);
    activityScreen->show();
    this->close();
}
void Goals::on_goalsButton_clicked() { qDebug() << "Already on the Goals!"; }
void Goals::on_historyButton_clicked() {
    HistoryScreen* historyScreen = new HistoryScreen(loggedInUser);
    historyScreen->show();
    this->close();
}
void Goals::on_settingsButton_clicked() { qDebug() << "Settings coming soon..."; }
void Goals::on_supportButton_clicked() { qDebug() << "Support coming soon..."; }
void Goals::on_profileButton_clicked() { qDebug() << "User profile..."; }
void Goals::on_bellButton_clicked() { qDebug() << "No new notifications."; }

void Goals::setupScrollArea()
{
    QWidget* container = ui.scrollAreaGoals->widget();

    if (!container->layout()) {
        cardsLayout = new QVBoxLayout(container);
    }
    else {
        cardsLayout = qobject_cast<QVBoxLayout*>(container->layout());
    }

    cardsLayout->setAlignment(Qt::AlignTop);
    cardsLayout->setSpacing(15);
    cardsLayout->setContentsMargins(15, 15, 30, 15);

    ui.scrollAreaGoals->setWidgetResizable(true);
    ui.scrollAreaGoals->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.scrollAreaGoals->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui.scrollAreaGoals->setStyleSheet(R"(
        QScrollArea { border: none; background-color: transparent; }
        QScrollBar:vertical { border: none; background: #F1F5F9; width: 8px; border-radius: 4px; margin: 0px 0px 0px 0px; }
        QScrollBar::handle:vertical { background: #1558A8; min-height: 30px; border-radius: 4px; }
        QScrollBar::handle:vertical:hover { background: #0E3D73; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { border: none; background: none; height: 0px; }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }
    )");
}

void Goals::loadGoalsFromDatabase()
{
   
    if (cardsLayout != nullptr) {
        QLayoutItem* item;
        while ((item = cardsLayout->takeAt(0)) != nullptr) {
            if (item->widget()) { item->widget()->deleteLater(); }
            delete item;
        }
    }

    QList<GoalRecord> goalsList = dbManager.getGoals(loggedInUser.getUsername());
    int activeGoalsCount = goalsList.size();
    double totalPercentage = 0;

    QDate currentDate = QDate::currentDate();
    DailyMetrics todayMetrics = dbManager.getDailyMetrics(loggedInUser.getUsername(), currentDate);

    for (const GoalRecord& record : goalsList) {

        double currentDynamicStatus = record.baseline; 

        if (record.category == "Physical Activity") {
            currentDynamicStatus = todayMetrics.stepCount; 
        }
        else if (record.category == "Recovery") {
            currentDynamicStatus = todayMetrics.sleepHours + (todayMetrics.sleepMinutes / 60.0); 
        }
        else if (record.category == "Biometric Data") {
            user freshUser = dbManager.getUserDetails(loggedInUser.getUsername());
            currentDynamicStatus = freshUser.getWeight(); 
        }
        else if (record.category == "Nutritional Target") {
            QSqlQuery wq;
            wq.prepare("SELECT SUM(water_l) FROM NutritionLog WHERE username = :usr AND record_date = :date");
            wq.bindValue(":usr", loggedInUser.getUsername());
            wq.bindValue(":date", currentDate.toString("yyyy-MM-dd"));
            if (wq.exec() && wq.next()) {
                currentDynamicStatus = wq.value(0).toDouble();
            }
        }

        GoalsCard* card = new GoalsCard(ui.scrollAreaGoals->widget());

        card->setGoalData(record.targetName, record.category, record.baseline, currentDynamicStatus, record.threshold);

        card->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        card->setMinimumHeight(120);

        cardsLayout->addWidget(card);
        card->show();

        if (record.threshold != record.baseline) { 

            double pct = 0;
            pct = ((currentDynamicStatus - record.baseline) / (record.threshold - record.baseline)) * 100.0;

            if (pct > 100.0) pct = 100.0;
            if (pct < 0.0) pct = 0.0;

            totalPercentage += pct;
        }
    }

    ui.lblActiveGoalsValue->setText(QString("%1").arg(activeGoalsCount, 2, 10, QChar('0')));
    double avgCompletion = (activeGoalsCount > 0) ? (totalPercentage / activeGoalsCount) : 0;
    ui.lblCompletionRateValue->setText(QString::number(static_cast<int>(avgCompletion)) + "%");

    ui.scrollAreaGoals->widget()->adjustSize();
}

void Goals::onInitializeTargetClicked()
{
    QString targetName = ui.txtTargetName->text();
    QString category = ui.comboCategory->currentText();
    QString baselineStr = ui.txtBaseline->text();
    QString thresholdStr = ui.txtThreshold->text();
    QString frequency = getSelectedFrequency();

    if (targetName.isEmpty() || baselineStr.isEmpty() || thresholdStr.isEmpty()) {
        QMessageBox::warning(this, "Missing Data", "Please fill out all fields.");
        return;
    }

    double baseline = baselineStr.toDouble();
    double threshold = thresholdStr.toDouble();

    bool success = dbManager.saveGoal(loggedInUser.getUsername(), targetName, category, baseline, threshold, frequency);

    if (success) {
        clearForm();
        loadGoalsFromDatabase();
        QMessageBox::information(this, "Target Initialized", "Your new strategic target has been set.");
    }
    else {
        QMessageBox::critical(this, "Error", "Failed to save the new goal.");
    }
}

void Goals::clearForm()
{
    ui.txtTargetName->clear();
    ui.txtBaseline->clear();
    ui.txtThreshold->clear();
    ui.comboCategory->setCurrentIndex(0);
    setFrequency("Daily");
}