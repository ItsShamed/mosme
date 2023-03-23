//
// Created by Ryuki
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginWindow.h" resolved

#include "loginwindow.h"
#include "ui_LoginWindow.h"


LoginWindow::LoginWindow(QWidget* parent) :
        QDialog(parent), ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowTitle("mosme - Log in");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
