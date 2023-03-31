//
// Created by Ryuki
//

#pragma once

#include "../../Memo.h"
#include "../ReplyingAPIRequest.h"
#include "responses/GetMemosResponse.h"

namespace mosme
{

    class GetMemosRequest : public ReplyingAPIRequest<GetMemosResponse>
    {
    Q_OBJECT
    REQUEST_GET_OPERATION

    REQUEST_REQUIRES_LOGIN(true)
    
    private:
        MemoVisibility rowStatus;
        int offset;
        int limit;
        
    protected:
        
        string GetTarget() const override;
        
        GetMemosResponse* CreateResponseData(QByteArray arr) override;
        
        GetMemosRequest(MemoVisibility & = Normal, int = 0, int = 20);

    };
} // mosme
