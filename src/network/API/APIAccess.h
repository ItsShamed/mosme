//
// Created by Ryuki
//

#pragma once

#include <QNetworkAccessManager>
#include <fmt/format.h>
#include <queue>
#include "APIRequest.h"
#include "../../ConfigStorage.h"
#include "APIState.h"

using namespace fmt;

namespace mosme
{


    class APIAccess : public QNetworkAccessManager, public IAPIProvider
    {
    Q_OBJECT

        friend class APIRequest;

    private:
        queue<APIRequest> queue;
        ConfigStorage config;
        string username;
        string password;
        APIState state;

        void run();

        void processQueuedRequests();

        void attemptConnect();
        
        void handleRequest(APIRequest&);
        
        void flushQueue(bool&);

    signals:

    public:
        void Perform(APIRequest*) override;
    
        void Login(string& username, string& password) override;
        
        void Logout() override;

        virtual bool IsLoggedIn() const;

        virtual string GetProvidedUsername() const;

        virtual APIState GetState() const;
        
        void Queue(APIRequest&) override;

    public:
        APIAccess() = delete;

        explicit APIAccess(ConfigStorage &);
    };
} // mosme
