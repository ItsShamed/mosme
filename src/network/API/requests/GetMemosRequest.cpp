//
// Created by Ryuki
//

#include <fmt/format.h>
#include "GetMemosRequest.h"

using namespace fmt;

namespace mosme
{
    GetMemosRequest::GetMemosRequest(MemoVisibility &rowStatus, int offset, int limit) 
        : rowStatus(rowStatus), offset(offset), limit(limit)
    {
    }
    
    string GetMemosRequest::GetTarget() const
    {
        return format("memo?rowStatus={0}&offset={1}&limit={2}", 
                      MemoUtils::VisibilityToString(rowStatus), offset, limit);
    }
    
    GetMemosResponse* CreateResponseData(QByteArray arr)
    {
        GetMemosResponse* resp = new GetMemosResponse;
        json j = JsonUtils::QByteArrayToJson(arr);
        *resp = j.get<GetMemosResponse>();
        return resp;
    }
} // mosme