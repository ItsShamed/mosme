//
// Created by Ryuki
//

#pragma once

#include "../../User.h"
#include "../ReplyingAPIRequest.h"

namespace mosme
{

    class GetUserMeRequest : public ReplyingAPIRequest<User>
    {
    Q_OBJECT
    
    REQUEST_GET_OPERATION

    REQUEST_REQUIRES_LOGIN(true)

    REQUEST_TARGET("user/me")
    
    protected:
    private:
        User* CreateResponseData(QByteArray arr) override;
    };
} // mosme
