//
// Created by Ryuki
//

#include "../APIAccess.h"
#include "GetStatusRequest.h"
#include "../../../json/JsonUtils.h"

namespace mosme
{
    GetStatusResponse* GetStatusRequest::CreateResponseData(QByteArray b)
    {
        GetStatusResponse* resp = new GetStatusResponse;
        json j = JsonUtils::QByteArrayToJson(b);
        *resp = j.get<GetStatusResponse>();
        return resp;
    }
} // mosme