//
// Created by Ryuki
//

#include "../APIAccess.h"
#include "GetStatusRequest.h"

namespace mosme
{
    GetStatusResponse* GetStatusRequest::CreateResponseData(QByteArray b)
    {
        GetStatusResponse* resp;
        *resp = json::parse(b.toStdString())["data"].get<GetStatusResponse>();
        return resp;
    }
} // mosme