//
// Created by Ryuki
//

#include <fmt/format.h>
#include <QMutexLocker>
#include <QNetworkReply>
#include <QEventLoop>
#include "APIAccess.h"
#include "APIRequest.h"

using namespace fmt;

namespace mosme
{
    APIRequest::APIRequest() : QObject(nullptr)
    {
    }

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

        this->moveToThread(API->thread());

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
                qDebug() << "HEAD request";
                Reply = API->network->head(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::GetOperation:
                qDebug() << "GET request";
                Reply = API->network->get(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::DeleteOperation:
                qDebug() << "DELETE request";
                Reply = API->network->deleteResource(*WebRequest);
                break;
            case QNetworkAccessManager::Operation::PostOperation:
                qDebug() << "POST request";
                Reply = API->network->post(*WebRequest, CreateRequestData());
                break;
            case QNetworkAccessManager::Operation::PutOperation:
                qDebug() << "PUT request";
                Reply = API->network->put(*WebRequest, CreateRequestData());
                break;
            default:
                Fail("Unsupported request operation.", nullptr);
                return;
        }

        Reply->moveToThread(API->network->thread());

        if (loop)
            loop->deleteLater();

        loop = new QEventLoop;

        connect(Reply, SIGNAL(finished()), loop, SLOT(quit()));
        connect(Reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), loop, SLOT(quit()));


        connect(Reply, &QNetworkReply::errorOccurred, this, [&](QNetworkReply::NetworkError code)
        {
            Fail(format("{0} failed with error code {1}: {2}",
                        typeid(this).name(), (int) code, Reply->errorString().toStdString()), new HttpCode{code});
        });

        connect(Reply, &QNetworkReply::downloadProgress, this, [&](qint64 d, qint64 t)
        {
            qDebug() << "Request progress: " << d << "/" << t;
        });

        loop->exec();

/*
        while (Reply->isRunning())
        {
            qDebug() << Reply->isTransactionStarted();
            qDebug() << "quoicoubeh";
            qDebug() << Reply->isFinished();
        }
*/

        if (isFailing()) return;

        qDebug() << "Post-processing request...";
        PostProcess();

        TriggerSuccess();
    }

    void APIRequest::PostProcess()
    {
    }

    void APIRequest::TriggerSuccess()
    {
        qDebug() << "Triggering request success...";
        {
            QMutexLocker lock(&completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Completed;
        }


        emit Success();
    }

    void APIRequest::TriggerFailure(const string &msg, const HttpCode* err)
    {
        {
            QMutexLocker lock(&completionStateLock);

            if (completionState != Waiting)
                return;

            completionState = Failed;
        }
        
        HttpCode errCpy{0};
        
        if (err)
            errCpy = *err;
        
        
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
        {
            QMutexLocker lock(&completionStateLock);

            if (completionState != Waiting)
                return;
        }
        Reply->abort();
        loop->quit();

        qCritical() << "Failing request " << typeid(this).name() << ": " << QByteArrayView(msg);
        TriggerFailure(msg, err);
    }

    bool APIRequest::isFailing()
    {
        QMutexLocker lock(&completionStateLock);
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