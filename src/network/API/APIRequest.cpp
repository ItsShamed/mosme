//
// Created by Ryuki
//

#include <fmt/format.h>
#include <QNetworkReply>
#include "APIRequest.h"

using namespace fmt;

namespace mosme
{
    string APIRequest::GetUri()
    {
        return format("{0}://{1}/api/{2}",
                      API->config.UseHttps ? "https" : "http",
                      API->config.Host,
                      GetTarget());
    }

    APIRequestCompletionState APIRequest::GetCompletionState()
    {
        return completionState;
    }

    void APIRequest::Perform(IAPIProvider* api)
    {
        if (api == nullptr || typeid(*api) != typeid(APIAccess))
        {
            Fail(format("A {} is required in order to make requests.", typeid(APIAccess).name()));
            return;
        }

        API = dynamic_cast<APIAccess*>(api);

        if (isFailing()) return;

        delete WebRequest;
        delete Reply;

        WebRequest = CreateWebRequest();

        qInfo() << "Performing request " << typeid(this).name();

        switch (GetOperation())
        {
            case QNetworkAccessManager::Operation::HeadOperation:
                Reply = API->head(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::GetOperation:
                Reply = API->get(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::DeleteOperation:
                Reply = API->deleteResource(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::PostOperation:
                Reply = API->post(*WebRequest, CreateRequestData());
                break;
            case QNetworkAccessManager::Operation::PutOperation:
                Reply = API->put(*WebRequest, CreateRequestData());
                break;
            default:
                Fail("Unsupported request operation.");
                return;
        }

        connect(Reply, &QNetworkReply::errorOccurred, this, [&](QNetworkReply::NetworkError code)
        {
            Fail(format("{0} failed with error code {1}: {2}",
                        typeid(this).name(), (int) code, Reply->errorString().toStdString()));
        });

        if (isFailing()) return;

        PostProcess();

        TriggerSuccess();
    }

    void APIRequest::PostProcess()
    {
    }

    void APIRequest::TriggerSuccess()
    {

        {
            lock_guard<mutex> lock(completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Completed;
        }


        emit Success();
    }

    void APIRequest::TriggerFailure(const string &msg)
    {
        {
            lock_guard<mutex> lock(completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Failed;
        }

        emit Failure(msg);
    }

    void APIRequest::Cancel()
    {
        Fail("Request cancelled.");
    }

    void APIRequest::Fail(const string &msg)
    {
        lock_guard<mutex> lock(completionStateLock);

        if (completionState != Waiting)
            return;

        Reply->abort();

        qCritical() << "Failing request " << typeid(this).name() << ": " << QByteArrayView(msg);
        TriggerFailure(msg);
    }

    bool APIRequest::isFailing()
    {
        lock_guard<mutex> lock(completionStateLock);
        return completionState == Failed;
    }

    template<typename T>
    ReplyingAPIRequest<T>::ReplyingAPIRequest()
    {
        connect(this, &APIRequest::Success, this, [&]()
        {
            emit Success(GetResponse());
        });
    }

    template<typename T>
    T* ReplyingAPIRequest<T>::GetResponse() const
    {
        return response;
    }

    template<typename T>
    void ReplyingAPIRequest<T>::PostProcess()
    {
        if (Reply != nullptr)
        {
            response = CreateData(Reply->readAll());
            qInfo() << "Finished with response size of " << Reply->size() << " bytes.";
        }

        APIRequest::PostProcess();
    }
} // mosme