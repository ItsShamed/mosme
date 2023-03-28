//
// Created by Ryuki
//

#pragma once

#include <mutex>
#include "ICanPerform.h"
#include "APIAccess.h"
#include "IAPIProvider.h"
#include "APIRequestCompletionState.h"

using namespace std;

namespace mosme
{
    class APIRequest : public QObject, public ICanPerform<IAPIProvider>
    {
    Q_OBJECT
    private:
        mutex completionStateLock;
        APIRequestCompletionState completionState = Waiting;

        bool isFailing();


    protected:
        virtual string GetTarget() = 0;

        virtual QNetworkAccessManager::Operation GetOperation() = 0;

        APIAccess* API = nullptr;
        QNetworkRequest* WebRequest = nullptr;
        QNetworkReply* Reply = nullptr;

        virtual QNetworkRequest* CreateWebRequest() const = 0;

        virtual QByteArray CreateRequestData() const = 0;

        virtual string GetUri();

        void TriggerFailure(const string &);

        void TriggerSuccess();

        virtual void PostProcess();

    signals:

        void Success();

        void Failure(const string &);

    public slots:

        void Perform(IAPIProvider*) override;

        void Cancel();

        void Fail(const string &);

    public:
        APIRequestCompletionState GetCompletionState();
    };

    template<typename T>
    class ReplyingAPIRequest : public APIRequest
    {
    private:
        T* response;
      
    protected:
        ReplyingAPIRequest();
        
        virtual T* CreateData(QByteArray&) = 0;
        
        void PostProcess() override;

    signals:

        void Success(T &);

    public:
        T* GetResponse() const;
    };
} // mosme
