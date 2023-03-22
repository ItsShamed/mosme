#include <QApplication>
#include <QPushButton>
#include <QStyleFactory>
#include <iostream>
#include "ui/loginwindow.h"
#include "MosmeApp.h"

using namespace std;
using namespace mosme;

int main(int argc, char* argv[])
{
    MosmeApp a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    LoginWindow loginWindow(nullptr);
    loginWindow.show();
    return QApplication::exec();
}
