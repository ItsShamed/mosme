//
// Created by Ryuki
//

#pragma once

#include <QDialog>
#include "../ConfigStorage.h"
#include "../network/API/APIAccess.h"

namespace mosme
{
    QT_BEGIN_NAMESPACE
    namespace Ui { class LoginWindow; }
    QT_END_NAMESPACE

    class LoginWindow : public QDialog
    {
    Q_OBJECT

    public:
        LoginWindow(ConfigStorage* config, APIAccess* api, QWidget* parent = nullptr);

        ~LoginWindow() override;
        
    private slots:
        void login();

    private:
        Ui::LoginWindow* ui;
        ConfigStorage* config;
        APIAccess* api;
    };
}