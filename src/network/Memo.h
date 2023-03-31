//
// Created by Ryuki
//

#pragma once

#include <string>
#include <stdexcept>
#include <fmt/format.h>
#include <QDateTime>
#include <nlohmann/json.hpp>

using namespace std;
using namespace fmt;
using namespace nlohmann;

namespace mosme
{
    enum MemoVisibility
    {
        Normal,
        Protected,
        Private
    };

    struct Memo
    {
        int id;
        MemoVisibility rowStatus; // don't know what this does but it's in the response so...
        int creatorId;
        QDateTime created;
        QDateTime updated;
        string content;
        MemoVisibility visibility;
        bool pinned;
        string creatorName;
    };


    void from_json(const json &, Memo &);

    class MemoUtils
    {
    public:
        static inline string VisibilityToString(MemoVisibility visibility)
        {
            switch (visibility)
            {
                case Normal:
                    return "NORMAL";
                case Protected:
                    return "PROTECTED";
                case Private:
                    return "PRIVATE";
            }
        }

        static inline MemoVisibility StringToVisibility(const string &str)
        {
            if (str == "NORMAL")
                return Normal;
            if (str == "PROTECTED")
                return Protected;
            if (str == "PRIVATE")
                return Private;

            throw invalid_argument(format("The string \"{}\" does not represent a visibility", str));
        }
    };
}

W_REGISTER_ARGTYPE(mosme::Memo)
W_REGISTER_ARGTYPE(mosme::Memo &)

W_REGISTER_ARGTYPE(vector<mosme::Memo>)
W_REGISTER_ARGTYPE(vector<mosme::Memo> &)

W_REGISTER_ARGTYPE(vector<mosme::Memo*>)
W_REGISTER_ARGTYPE(vector<mosme::Memo*> &)
