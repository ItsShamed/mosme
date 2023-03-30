//
// Created by Ryuki
//

#pragma once

#include "responses/GetStatusResponse.h"
#include "../ReplyingAPIRequest.h"

namespace mosme
{


    class GetStatusRequest : public ReplyingAPIRequest<GetStatusResponse>
    {
    Q_OBJECT
    REQUEST_GET_OPERATION

    REQUEST_REQUIRES_LOGIN(false)

    REQUEST_TARGET("status")

    protected:
        
        GetStatusResponse* CreateResponseData(QByteArray b) override;
    };
} // mosme
