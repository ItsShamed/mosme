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
        User* resp = new User;
        *resp = JsonUtils::QByteArrayToJson(arr).at("data").get<User>();
        return resp;
    }

    QNetworkRequest* PostAuthSignInRequest::CreateWebRequest() const
    {
        QNetworkRequest* req = APIRequest::CreateWebRequest();
        req->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        return req;
    }
} // mosme