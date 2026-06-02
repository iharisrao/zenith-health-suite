# ZenithHealthSuite 🩺📊

**ZenithHealthSuite** is an offline-first, data-driven desktop health tracking application. Built with **C++** and the **Qt Framework**, it provides a centralized platform for users to monitor their vitals, track daily activities, log nutritional intake, and set dynamic health goals without relying on internet connectivity or third-party cloud servers.

## 🚀 Key Features
* **Real-Time Dashboard:** Instant visualization of daily calories, step counts, active energy, and sleep recovery.
* **Vitals & Activity Logging:** Securely record weight, heart rate, blood pressure, blood sugar, and workout intensities.
* **Goal Management:** Set and track dynamic baselines and thresholds for continuous health improvement.
* **100% Offline & Private:** All data is stored locally on the device using an embedded database, ensuring absolute data privacy and zero subscription costs.

## 🛠️ Technical Stack
* **Language:** Standard C++
* **UI Framework:** Qt Framework (Qt Widgets, Qt Charts)
* **Database Engine:** SQLite (Embedded, Serverless)

## 🗄️ Database Architecture & Optimization
This project was designed with a strong focus on Database Management System (DBMS) principles, prioritizing data integrity, security, and high performance:

* **Pre-Aggregation Caching (`DailyMetrics`):** To prevent the UI from lagging when analyzing thousands of raw logs, the C++ backend pre-calculates daily totals (using SQL `SUM()` and `AVG()`) and caches them in a dedicated `DailyMetrics` table. This reduces dashboard loading time complexity from $O(n)$ to $O(1)$.
* **Data Integrity via Transactions:** Multi-step write operations and table initializations are wrapped in SQL Transactions (`db.transaction()`, `db.commit()`, `db.rollback()`) to ensure atomicity and prevent partial database states.
* **SQL Injection Prevention:** All user inputs are strictly bound to queries using Prepared Statements and parameterized placeholders.
* **Relational Schema:** A purpose-built 6-table schema (`User`, `DailyMetrics`, `ActivityLog`, `VitalsLog`, `NutritionLog`, `UserGoals`) managing one-to-many relationships through application-level logic.

## ⚙️ Getting Started
To run this project locally on your machine:

1. **Prerequisites:** Install [Qt Creator](https://www.qt.io/download) and the Qt Framework. Ensure the MSVC (Microsoft Visual C++) compiler is configured.
2. **Clone the repository:**
   ```bash
   git clone [[https://github.com/iharisrao/zenith-health-suite.git](https://github.com/iharisrao/zenith-health-suite.git)]
