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
        j.at("id").get_to(u.id);

        QDateTime ctime;
        ctime.setMSecsSinceEpoch(j.at("createdTs").get<long>());
        u.created = ctime;

        QDateTime utime;
        utime.setMSecsSinceEpoch(j.at("updatedTs").get<long>());
        u.updated = utime;

        j.at("username").get_to(u.username);
        j.at("role").get_to(u.role);
        j.at("email").get_to(u.email);
        j.at("nickname").get_to(u.nickname);
        j.at("openId").get_to(u.openId);

        u.avatarUrl = QUrl(j.at("avatarUrl").get<string>().c_str());
    }

    // Json conversion for struct GetStatusResponse
    void from_json(const json &j, GetStatusResponse &r)
    {
        json data = j.at("data");

        data.at("host").get_to(r.host);
        data.at("profile").get_to(r.profile);
        data.at("allowSignUp").get_to(r.allowSignUp);
        data.at("disablePublicMemos").get_to(r.disablePublicMemos);
        data.at("customizedProfile").get_to(r.customisedProfile);
    }
    
    // Json conversion for struct HostCustomisedProfile
    void from_json(const json& j, HostCustomisedProfile& profile)
    {
        j.at("name").get_to(profile.name);
        j.at("description").get_to(profile.description);
        j.at("locale").get_to(profile.locale);
        j.at("appearance").get_to(profile.appearance);
    }

    // Json conversion for struct HostProfile
    void from_json(const json& j, HostProfile& profile)
    {
        j.at("mode").get_to(profile.mode);
        j.at("version").get_to(profile.version);
    }
}