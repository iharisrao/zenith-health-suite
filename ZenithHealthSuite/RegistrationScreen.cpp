#include "RegistrationScreen.h"
#include "ui_RegistrationScreen.h"  //it is seceret file which is created by Qt when we use drag and drop canvas
#include <QMessageBox>              // tool which is used to make pop up alerts and show on screen

RegistrationScreen::RegistrationScreen(QWidget *parent):
QWidget(parent),
ui(new Ui::RegistrationScreen)    //same remote control pointer used in .h file to control the ui elements on the registration screen
{
	ui->setupUi(this);            // magic line which creates the physical lines and boxes on computer screen from your drag and drop design 
}

RegistrationScreen::~RegistrationScreen()
{
	delete ui;
}

void RegistrationScreen::on_completeRegistrationButton_clicked()
{
	QMessageBox::information(this, "System Status", "The complete registration button is alive and connected!"); 
	// Pop up a success message box right in the middle of this screen
}