//
// Created by Ryuki
//

#pragma once

#include <exception>
#include <string>

using namespace std;

namespace mosme
{

    class NotSupportedException : exception
    {
    public:
        string Message = "";

        explicit NotSupportedException(string);
    };
} // mosme
