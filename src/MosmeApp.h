//
// Created by Ryuki
//

#pragma once

#include <QApplication>

namespace mosme
{

    class MosmeApp : public QApplication
    {
    public:
        MosmeApp(int& argc, char* argv[]);
        void Hello() const;
    };
} // mosme
