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
        User* resp;
        *resp = JsonUtils::QByteArrayToJson(arr)["data"].get<User>();
        return resp;
    }
} // mosme