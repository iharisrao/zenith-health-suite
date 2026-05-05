#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ZenithHealthSuite.h"

class ZenithHealthSuite : public QMainWindow
{
    Q_OBJECT

public:
    ZenithHealthSuite(QWidget *parent = nullptr);
    ~ZenithHealthSuite();

private:
    Ui::ZenithHealthSuiteClass ui;
};

