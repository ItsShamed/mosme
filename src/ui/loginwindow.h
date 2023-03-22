//
// Created by Ryuki
//

#pragma once

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

    ~LoginWindow() override;

private:
    Ui::LoginWindow* ui;
};
