//
// Created by Ryuki
//

#pragma once

#include <QDateTime>
#include <wobjectimpl.h>
#include <nlohmann/json.hpp>
#include <QUrl>

using namespace std;
using namespace nlohmann;

namespace mosme
{
    struct User
    {
        int id;
        QDateTime created;
        QDateTime updated;
        string username;
        string role;
        string email;
        string nickname;
        string openId;
        QUrl avatarUrl;
    };
    
    void from_json(const json& j, User& u);
/*
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
*/
}

W_REGISTER_ARGTYPE(mosme::User)

W_REGISTER_ARGTYPE(mosme::User&)
