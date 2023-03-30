//
// Created by Ryuki
//

#pragma once

#include "../../User.h"
#include "../ReplyingAPIRequest.h"

namespace mosme
{

    class PostAuthSignInRequest : public ReplyingAPIRequest<User>
    {
    Q_OBJECT
    REQUEST_POST_OPERATION

    REQUEST_REQUIRES_LOGIN(false)

    REQUEST_TARGET("auth/signin")

    private:
        string username;
        string password;

    protected:
        QByteArray CreateRequestData() const override;

        User* CreateResponseData(QByteArray arr) override;
   
    public:
        PostAuthSignInRequest() = delete;

        PostAuthSignInRequest(const string &username, const string &password);
    };
} // mosme
