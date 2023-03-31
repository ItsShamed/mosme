//
// Created by Ryuki
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QThreadPool>
#include "mainwindow.h"
#include "ui_MainWindow.h"

namespace mosme
{
    MainWindow::MainWindow(QWidget* parent) :
            QMainWindow(parent), ui(new Ui::MainWindow), config("config.dat"), login(&config, &api, this),
            api(&config)
    {
        ui->setupUi(this);

        connect(ui->actionEdit_connection, &QAction::triggered, this, [&](bool)
        {
           login.show(); 
        });
        
        connect(ui->actionExit, &QAction::triggered, this, [&](bool)
        {
            this->close();
        });
        
//        api.Start();
        
//        QThreadPool::globalInstance()->start([this](){api = new APIAccess(&config);});
    }

    MainWindow::~MainWindow()
    {
        api.Stop();
        delete ui;
    }
} // mosme
