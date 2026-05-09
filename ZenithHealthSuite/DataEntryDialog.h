#ifndef DataEntryDialog_h
#define DataEntryDialog_h

#include<QDialog>
#include<QDate>
#include<QDebug>

namespace Ui {
	class DataEntryDialog;
}

class DataEntryDialog : public QDialog {
	Q_OBJECT
public:
	explicit DataEntryDialog(QWidget* parent = nullptr);
	~DataEntryDialog();

	void setPreviousWeight(double lastWeight);
	
private slots:
	void on_buttonBox_accepted();

private:
	Ui::DataEntryDialog* ui;
};

#endif