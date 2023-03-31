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
#include "APILoopThread.h"


#ifndef QT_NO_DEBUG
#define CHECK_TRUE(instruction) Q_ASSERT(instruction)
#else
#define CHECK_TRUE(instruction) (instruction)
#endif

using namespace fmt;

namespace mosme
{

    class APIAccess : public QObject, public IAPIProvider
    {
    Q_OBJECT

        friend class APIRequest;

        friend class APILoopThread;

        Q_PROPERTY(APIState currentState MEMBER state NOTIFY stateChanged)

    private:

        queue<APIRequest*> queue;
        ConfigStorage* config;
        string host;
        string username;
        string password;
        APIState state;
        APILoopThread loopThread;
        QNetworkAccessManager* network;
        QNetworkCookieJar* networkCookieJar;
        mutex queueLock;
        char failCount = 0;
        bool useCredentials = false;
        bool isInterruptionRequested = false;

        void run();

        bool handleRequest(APIRequest &);

        void handleFailure();

        void handleWebException(HttpCode &code);

        void flushQueue(bool = true);
        
    private slots:

        void processQueuedRequests();

        void attemptConnect();

    protected:
        string GetSessionCookie() const;

        bool IsCookieValid() const;

    signals:
        void stateChanged(const APIState state);

    public:
        void Perform(APIRequest*) override;

        void Login(string &username, string &password) override;

        void Logout() override;

        virtual bool IsLoggedIn() const;

        virtual string GetProvidedUsername() const;

        virtual APIState GetState() const;

        void Queue(APIRequest &) override;

        void SetHost(const string &host);
        
        void Stop();
        
        void Start();

    public:
        APIAccess() = delete;

        explicit APIAccess(ConfigStorage*);

        ~APIAccess() override;
    };
} // mosme
