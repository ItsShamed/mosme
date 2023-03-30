//
// Created by Ryuki
//

#pragma once

#include <QThread>
#include <QNetworkAccessManager>
#include <fmt/format.h>
#include <queue>
#include "APIRequest.h"
#include "../../ConfigStorage.h"
#include "APIState.h"

using namespace fmt;

namespace mosme
{
    class WorkerThread;

    class APIAccess : public QNetworkAccessManager, public IAPIProvider
    {
    Q_OBJECT

        friend class APIRequest;

    private:

        friend class WorkerThread;

        queue<APIRequest*> queue;
        ConfigStorage* config;
        string host;
        string username;
        string password;
        APIState state;
        WorkerThread* thread;
        QNetworkCookieJar* networkCookieJar;
        mutex queueLock;
        char failCount = 0;
        bool useCredentials = false;

        void processQueuedRequests();

        void attemptConnect();

        bool handleRequest(APIRequest &);
        
        void handleFailure();
        
        void handleWebException(HttpCode& code);

        void flushQueue(bool = true);

    protected:
        string GetSessionCookie() const;
        
        bool IsCookieValid() const;

    signals:

    public:
        void Perform(APIRequest*) override;

        void Login(string &username, string &password) override;

        void Logout() override;

        virtual bool IsLoggedIn() const;

        virtual string GetProvidedUsername() const;

        virtual APIState GetState() const;

        void Queue(APIRequest &) override;
        
        void SetHost(const string &host);

    public:
        APIAccess() = delete;

        explicit APIAccess(ConfigStorage*);

        ~APIAccess() override;
    };

    class WorkerThread : public QThread
    {
    private:
        APIAccess* $this;

    public:
        void run() override;

    public:
        explicit WorkerThread(APIAccess*);
    };
} // mosme
