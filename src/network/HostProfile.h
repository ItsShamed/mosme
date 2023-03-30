//
// Created by Ryuki
//

#pragma once

#include <string>
#include <wobjectimpl.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

namespace mosme
{
    struct HostProfile
    {
        string mode;
        string version;
    };
    
    void from_json(const json& j, HostProfile& profile);
/*
    {
        j["mode"].get_to(profile.mode);
        j["version"].get_to(profile.version);
    }
*/
}

W_REGISTER_ARGTYPE(mosme::HostProfile)

W_REGISTER_ARGTYPE(mosme::HostProfile&)
