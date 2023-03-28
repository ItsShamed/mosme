//
// Created by Ryuki
//

#pragma once

#include <string>
#include "APIRequest.h"
#include "APIAccess.h"
#include "APIState.h"

using namespace std;

namespace mosme
{
    class APIRequest;
    class IAPIProvider : ICanPerform<APIRequest>
    {
    public:
        IAPIProvider() = delete;
        virtual bool IsLoggedIn() const = 0;
        virtual string GetProvidedUsername() const = 0;
        virtual APIState GetState() const = 0;
        
        virtual void Queue(APIRequest&) = 0;
        virtual void Login(string& username, string& password) = 0;
        virtual void Logout() = 0;
    };
} // mosme
