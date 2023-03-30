//
// Created by Ryuki
//

#pragma once

#include <QByteArray>
#include <wobjectcpp.h>
#include <wobjectimpl.h>
#include "APIRequest.h"

namespace mosme
{

    template<typename T>
    class ReplyingAPIRequest : public APIRequest
    {
        W_OBJECT(ReplyingAPIRequest)
    private:
        T* response;

    protected:
        virtual T* CreateResponseData(QByteArray) = 0;

        void PostProcess() override
        {

            if (Reply != nullptr)
            {
                response = CreateResponseData(Reply->readAll());
                qInfo() << "Finished with response size of " << Reply->size() << " bytes.";
            }

            APIRequest::PostProcess();
        };

    public: // signals

        void Success(T &t) W_SIGNAL(Success, t)

    public:
        ReplyingAPIRequest()
        {
            connect(this, &APIRequest::Success, this, [&]()
            {
                emit Success(*GetResponse());
            });
        };

        T* GetResponse() const
        {
            return response;
        };
    };

} // mosme

W_OBJECT_IMPL(mosme::ReplyingAPIRequest<T>, template<typename T>)
