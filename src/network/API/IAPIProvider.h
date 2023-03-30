//
// Created by Ryuki
//

#pragma once

#include <string>
#include "APIState.h"

using namespace std;

namespace mosme
{
    class APIRequest;

    class IAPIProvider : ICanPerform<APIRequest>
    {
    protected:
        IAPIProvider() = default;

    public:
        inline virtual bool IsLoggedIn() const = 0;

        inline virtual string GetProvidedUsername() const = 0;

        inline virtual APIState GetState() const = 0;

        virtual void Queue(APIRequest &) = 0;

        virtual void Login(string &username, string &password) = 0;

        virtual void Logout() = 0;
    };
} // mosme
