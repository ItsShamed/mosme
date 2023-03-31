//
// Created by Ryuki
//

// You may need to build the project (run Qt uic code generator) to get "ui_LoginWindow.h" resolved

#include <QMessageBox>
#include "loginwindow.h"
#include "ui_LoginWindow.h"


namespace mosme
{
    LoginWindow::LoginWindow(ConfigStorage* config, APIAccess* api, QWidget* parent) :
            QDialog(parent), ui(new Ui::LoginWindow)
    {
        qDebug() << "Login window";
        ui->setupUi(this);
        setWindowTitle("mosme - Log in");

        this->config = config;
        this->api = api;

        connect(ui->LogInButton, &QPushButton::clicked, this, &LoginWindow::login);
        
        connect(ui->CancelButton, &QPushButton::clicked, this, [this]()
        {
            close();
        });
        
        ui->RememberCheck->setChecked(config->PersistentStorage);
        ui->UseHttpsCheck->setChecked(config->UseHttps);
        ui->GuestInput->setChecked(config->Guest);
        ui->HostInput->setText(QString(config->Host.c_str()));
        ui->UsernameInput->setText(QString(config->Username.c_str()));
        ui->PasswordInput->setText(QString(config->Password.c_str()));
    }

    LoginWindow::~LoginWindow()
    {
        delete ui;
    }

    void LoginWindow::login()
    {
        setDisabled(true);
        api->Logout();
        config->Host = ui->HostInput->text().toStdString();
        config->UseHttps = ui->UseHttpsCheck->isChecked();
        config->Guest = ui->GuestInput->isChecked();
        config->PersistentStorage = ui->RememberCheck->isChecked();
        if (!config->Guest)
        {
            if (ui->UsernameInput->text().isEmpty() || ui->PasswordInput->text().isEmpty())
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Login error");
                msgBox.setInformativeText("The username and password cannot be empty.");
                msgBox.setText("Invalid credentials");
                setDisabled(false);
                return;
            }
            config->Username = ui->UsernameInput->text().toStdString();
            config->Password = ui->PasswordInput->text().toStdString();
            config->Save();
            api->Login(config->Username, config->Password);
        }

        setDisabled(false);
        config->Save();
    }
}