//
// Created by Ryuki
//

#pragma once

#include <wobjectimpl.h>
#include "../../../User.h"
#include "../../../HostProfile.h"
#include "../../../HostCustomisedProfile.h"

namespace mosme
{
    struct GetStatusResponse
    {
        User host;
        HostProfile profile;
        bool allowSignUp{};
        bool disablePublicMemos{};
        HostCustomisedProfile customisedProfile;
    };
    
    void from_json(const json& j, GetStatusResponse& r);
/*
    {
        json data = j["data"];
        
        data["host"].get_to(r.host);
        data["profile"].get_to(r.profile);
        data["allowSignUp"].get_to(r.allowSignUp);
        data["disablePublicMemos"].get_to(r.disablePublicMemos);
        data["customizedProfile"].get_to(r.customisedProfile);
    }
*/
}

W_REGISTER_ARGTYPE(mosme::GetStatusResponse)

W_REGISTER_ARGTYPE(mosme::GetStatusResponse&)
