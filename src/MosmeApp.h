//
// Created by Ryuki
//

#pragma once

#include <QApplication>
#include "ConfigStorage.h"
#include "network/API/APIAccess.h"

namespace mosme
{

    class MosmeApp : public QApplication
    {
    private:
        ConfigStorage config;
        APIAccess api;
    public:
        MosmeApp(int &argc, char* argv[]);

        ~MosmeApp() override;

        void Hello() const;
    };
} // mosme
