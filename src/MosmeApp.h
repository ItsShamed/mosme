//
// Created by Ryuki
//

#pragma once

#include <QApplication>
#include "ConfigStorage.h"

namespace mosme
{

    class MosmeApp : public QApplication
    {
    private:
        ConfigStorage config;
    public:
        MosmeApp(int& argc, char* argv[]);
        ~MosmeApp() override;
        void Hello() const;
    };
} // mosme
