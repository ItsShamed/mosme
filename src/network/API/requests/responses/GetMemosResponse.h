//
// Created by Ryuki
//

#pragma once

#include "../../../Memo.h"

namespace mosme
{
    struct GetMemosResponse
    {
        vector<Memo> data;
    };

    void from_json(const json &, GetMemosResponse &);
}