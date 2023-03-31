#include <QApplication>
#include <QPushButton>
#include <iostream>
#include "ui/loginwindow.h"
#include "MosmeApp.h"
#include "ui/mainwindow.h"

using namespace std;
using namespace mosme;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec();
}
