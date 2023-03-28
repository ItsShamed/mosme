//
// Created by Ryuki
//

#include <utility>
#include "NotSupportedException.h"

namespace mosme
{
    NotSupportedException::NotSupportedException(string msg)
    {
        Message = std::move(msg);
    }
} // mosme