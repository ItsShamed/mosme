#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "ui/loginwindow.h"
#include "MosmeApp.h"

using namespace std;
using namespace mosme;

int main(int argc, char* argv[])
{
//    MosmeApp a(argc, argv);
//    LoginWindow loginWindow(nullptr);
//    loginWindow.show();
//    return QApplication::exec();
    ConfigStorage config("test.dat");
    config.Host = "a";
    config.Username = "test";
    config.Password = "yest";
    config.Session = "huh";
    config.Guest = false;
    cout << config << endl;
    config.Save();
    config.Password = "other";
    cout << config << endl;
    config.Load();
    cout << config << endl;
}
