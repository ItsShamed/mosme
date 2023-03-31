//
// Created by Ryuki
//

#pragma once

#include <QMainWindow>
#include "../ConfigStorage.h"
#include "../network/API/APIAccess.h"
#include "loginwindow.h"

namespace mosme
{
    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QMainWindow
    {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

        ~MainWindow() override;

    private:
        Ui::MainWindow* ui;
        ConfigStorage config;
        APIAccess api;
        LoginWindow login;
    };
} // mosme
