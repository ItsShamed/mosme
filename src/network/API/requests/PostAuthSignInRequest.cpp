//
// Created by Ryuki
//

#include "../APIAccess.h"
#include "PostAuthSignInRequest.h"
#include "../../../json/JsonUtils.h"

namespace mosme
{
    PostAuthSignInRequest::PostAuthSignInRequest(const string &username, const string &password)
        : username(username), password(password), ReplyingAPIRequest<User>()
    {
    }

    QByteArray PostAuthSignInRequest::CreateRequestData() const
    {
        json j;
        j["username"] = username;
        j["password"] = password;
        
        return JsonUtils::JsonToQByteArray(j);
    }

    User* PostAuthSignInRequest::CreateResponseData(QByteArray arr)
    {
        User* resp;
        *resp = JsonUtils::QByteArrayToJson(arr)["data"].get<User>();
        return resp;
    }
} // mosme