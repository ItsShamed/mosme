//
// Created by Ryuki
//

#include <fmt/format.h>
#include <QNetworkReply>
#include "APIAccess.h"
#include "APIRequest.h"

using namespace fmt;

namespace mosme
{
    string APIRequest::GetUri() const
    {
        return format("{0}/api/{1}", API->config->GetInstanceBaseUrl(), GetTarget());
    }

    APIRequestCompletionState APIRequest::GetCompletionState()
    {
        return completionState;
    }

    void APIRequest::Perform(IAPIProvider* api)
    {
        if (api == nullptr || typeid(*api) != typeid(APIAccess))
        {
            Fail(format("A {} is required in order to make requests.", typeid(APIAccess).name()), nullptr);
            return;
        }

        API = dynamic_cast<APIAccess*>(api);

        if (!API->IsLoggedIn() && RequiresLogin())
            Fail("This request requires to be logged in.", nullptr);

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
                Fail("Unsupported request operation.", nullptr);
                return;
        }

        connect(Reply, &QNetworkReply::errorOccurred, this, [&](QNetworkReply::NetworkError code)
        {
            Fail(format("{0} failed with error code {1}: {2}",
                        typeid(this).name(), (int) code, Reply->errorString().toStdString()), new HttpCode{code});
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
            lock_guard lock(completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Completed;
        }


        emit Success();
    }

    void APIRequest::TriggerFailure(const string &msg, const HttpCode* err)
    {
        {
            lock_guard lock(completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Failed;
        }
        HttpCode errCpy = *err;
        failure = new APIRequestFailure{
            msg,
            &errCpy
        };

        emit Failure(msg, err);
    }

    void APIRequest::Cancel()
    {
        Fail("Request cancelled.", nullptr);
    }

    void APIRequest::Fail(const string &msg, const HttpCode* err)
    {
        lock_guard lock(completionStateLock);

        if (completionState != Waiting)
            return;

        Reply->abort();

        qCritical() << "Failing request " << typeid(this).name() << ": " << QByteArrayView(msg);
        TriggerFailure(msg, err);
    }

    bool APIRequest::isFailing()
    {
        lock_guard lock(completionStateLock);
        return completionState == Failed;
    }

    QNetworkRequest* APIRequest::CreateWebRequest() const
    {
        return new QNetworkRequest(QUrl(QString(GetUri().c_str())));
    }

    APIRequestFailure* APIRequest::GetFailure() const
    {
        return failure;
    }
} // mosme