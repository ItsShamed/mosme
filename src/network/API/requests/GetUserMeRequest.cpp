//
// Created by Ryuki
//

#include "../APIAccess.h"
#include "GetUserMeRequest.h"
#include "../../../json/JsonUtils.h"

namespace mosme
{
    User* GetUserMeRequest::CreateResponseData(QByteArray arr)
    {
        User* resp = new User;
        json j = JsonUtils::QByteArrayToJson(arr).at("data");
        *resp = j.get<User>();
        return resp;
    }
} // mosme