//
// Created by Ryuki
//

#include "MosmeApp.h"
#include <iostream>

using namespace std;

namespace mosme
{
    MosmeApp::MosmeApp(int &argc, char** argv) : QApplication(argc, argv)
    {
        cout << "Hello" << endl;
    }

    void MosmeApp::Hello() const
    {
        cout << "Hello, this is the MosmeApp" << endl;
    }
} // mosme