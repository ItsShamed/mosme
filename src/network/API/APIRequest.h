//
// Created by Ryuki
//

#pragma once

#include <mutex>
#include <QNetworkReply>
#include "ICanPerform.h"
#include "APIAccess.h"
#include "IAPIProvider.h"
#include "APIRequestCompletionState.h"

#define REQUEST_REQUIRES_LOGIN(required) protected: bool RequiresLogin() const override { return required; }

#define REQUEST_GET_OPERATION protected: QNetworkAccessManager::Operation GetOperation() const override { \
return QNetworkAccessManager::Operation::GetOperation; }
#define REQUEST_POST_OPERATION protected: QNetworkAccessManager::Operation GetOperation() const override { \
return QNetworkAccessManager::Operation::PostOperation; }
#define REQUEST_PUT_OPERATION protected: QNetworkAccessManager::Operation GetOperation() const override { \
return QNetworkAccessManager::Operation::PutOperation; }
#define REQUEST_HEAD_OPERATION protected: QNetworkAccessManager::Operation GetOperation() const override { \
return QNetworkAccessManager::Operation::HeadOperation; }
#define REQUEST_DELETE_OPERATION protected: QNetworkAccessManager::Operation GetOperation() const override { \
return QNetworkAccessManager::Operation::DeleteOperation; }

#define REQUEST_TARGET(target) protected: string GetTarget() const override { return target; }


using namespace std;

namespace mosme
{
    union HttpCode
    {
        int code;
        QNetworkReply::NetworkError err;
    };
    
    struct APIRequestFailure
    {
        string msg;
        HttpCode* code;
    };
   
    class APIAccess;

    class APIRequest : public QObject, public ICanPerform<IAPIProvider>
    {
    Q_OBJECT
    private:
        mutex completionStateLock;
        APIRequestCompletionState completionState = Waiting;
        APIRequestFailure* failure;

        bool isFailing();


    protected:
        virtual string GetTarget() const = 0;

        virtual QNetworkAccessManager::Operation GetOperation() const = 0;

        virtual bool RequiresLogin() const = 0;

        APIAccess* API = nullptr;
        QNetworkRequest* WebRequest = nullptr;
        QNetworkReply* Reply = nullptr;

        virtual QNetworkRequest* CreateWebRequest() const;

        virtual QByteArray CreateRequestData() const { return {}; };

        virtual string GetUri() const;

        void TriggerFailure(const string &msg, const HttpCode*);

        void TriggerSuccess();

        virtual void PostProcess();

    signals:

        void Success();

        void Failure(const string &, const HttpCode*);

    public slots:

        void Perform(IAPIProvider*) override;

        void Cancel();

        void Fail(const string &msg, const HttpCode*);

    public:
        APIRequestCompletionState GetCompletionState();
        
        APIRequestFailure* GetFailure() const;
        
    };
} // mosme
