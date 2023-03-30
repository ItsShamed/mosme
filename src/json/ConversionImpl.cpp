//
// Created by Ryuki
//

#include "../network/HostCustomisedProfile.h"
#include "../network/HostProfile.h"
#include "../network/User.h"
#include "../network/API/requests/responses/GetStatusResponse.h"

namespace mosme
{
    // Json conversion for struct User
    void from_json(const json &j, User &u)
    {
        j["id"].get_to(u.id);

        QDateTime ctime;
        ctime.setMSecsSinceEpoch(j["created"].get<long>());
        u.created = ctime;

        QDateTime utime;
        utime.setMSecsSinceEpoch(j["updated"].get<long>());
        u.updated = utime;

        j["username"].get_to(u.username);
        j["role"].get_to(u.role);
        j["email"].get_to(u.email);
        j["nickname"].get_to(u.nickname);
        j["openId"].get_to(u.openId);

        u.avatarUrl = QUrl(j["avatarUrl"].get<string>().c_str());
    }

    // Json conversion for struct GetStatusResponse
    void from_json(const json &j, GetStatusResponse &r)
    {
        json data = j["data"];

        data["host"].get_to(r.host);
        data["profile"].get_to(r.profile);
        data["allowSignUp"].get_to(r.allowSignUp);
        data["disablePublicMemos"].get_to(r.disablePublicMemos);
        data["customizedProfile"].get_to(r.customisedProfile);
    }
    
    // Json conversion for struct HostCustomisedProfile
    void from_json(const json& j, HostCustomisedProfile& profile)
    {
        j["name"].get_to(profile.name);
        j["description"].get_to(profile.description);
        j["locale"].get_to(profile.locale);
        j["appearance"].get_to(profile.appearance);
    }

    // Json conversion for struct HostProfile
    void from_json(const json& j, HostProfile& profile)
    {
        j["mode"].get_to(profile.mode);
        j["version"].get_to(profile.version);
    }
}