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
    struct HostCustomisedProfile
    {
        string name;
        string description;
        string locale;
        string appearance;
    };
    
    
    void from_json(const json& j, HostCustomisedProfile& profile);
/*
    {
        j["name"].get_to(profile.name);
        j["description"].get_to(profile.description);
        j["locale"].get_to(profile.locale);
        j["appearance"].get_to(profile.appearance);
    }
*/
}

W_REGISTER_ARGTYPE(mosme::HostCustomisedProfile)

W_REGISTER_ARGTYPE(mosme::HostCustomisedProfile&)
