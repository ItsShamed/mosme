//
// Created by Ryuki
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

namespace mosme
{
    MainWindow::MainWindow(QWidget* parent) :
            QMainWindow(parent), ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
} // mosme