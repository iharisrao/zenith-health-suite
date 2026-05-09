#pragma once

#include <QDialog>
#include "ui_DataEntryDialog.h"

class DataEntryDialog : public QDialog
{
	Q_OBJECT

public:
	DataEntryDialog(QWidget *parent = nullptr);
	~DataEntryDialog();

private:
	Ui::DataEntryDialogClass ui;
};

