//
// Created by Ryuki
//

#include <QNetworkCookieJar>
#include <QtConcurrent>
#include "APIAccess.h"
#include "requests/GetStatusRequest.h"
#include "requests/PostAuthSignInRequest.h"
#include "requests/GetUserMeRequest.h"

namespace mosme
{
    APIAccess::APIAccess(ConfigStorage* configStorage)
            : username(configStorage->Username), password(configStorage->Password), host(configStorage->Host),
              network(new QNetworkAccessManager), networkCookieJar(new QNetworkCookieJar), loopThread(this)
    {
        qDebug() << "Starting API client...";
        config = configStorage;
        state = Offline;
        
        qDebug() << connect(&loopThread, &APILoopThread::callRequest_processQueuedRequests, this,
                            &APIAccess::processQueuedRequests);

        qDebug() << connect(&loopThread, &APILoopThread::callRequest_attemptConnect, this,
                            &APIAccess::attemptConnect);

        loopThread.start();
    }

    string APIAccess::GetSessionCookie() const
    {
        for (QNetworkCookie &cookie
                : networkCookieJar->cookiesForUrl(QUrl(QString(config->GetInstanceBaseUrl().append("/").c_str()))))
        {
            if (cookie.name() == QString("memo_session"))
                return cookie.value().toStdString();
        }
        return "";
    }

    bool APIAccess::IsCookieValid() const
    {
        if (config->Guest) return true; // You don't need cookies for Guest

        for (QNetworkCookie &cookie
                : networkCookieJar->cookiesForUrl(QUrl(QString(config->GetInstanceBaseUrl().append("/").c_str()))))
        {
            if (cookie.name() == QString("memo_session") && !cookie.value().isEmpty())
                return true;
        }

        return false;
    }

    bool APIAccess::IsLoggedIn() const
    {
        return IsCookieValid() || !GetProvidedUsername().empty() && !password.empty();
    }

    void APIAccess::processQueuedRequests()
    {
        QMutexLocker functionLock(&loopThread.processQueuedRequestsLock);
        while (true)
        {
            APIRequest* req;

            {
                lock_guard lock(queueLock);

                if (queue.empty()) return;

                req = queue.front();
            }

            handleRequest(*req);
        }
        qDebug() << "Finished processing queued requests!";
    }

    void APIAccess::attemptConnect()
    {
        QMutexLocker functionLock(&loopThread.attemptConnectLock);
        qDebug() << "Attempting to connect to the API...";
        state = Connecting;

        if (config->UseHttps)
        {
            qDebug() << "HTTPS connection to " << config->GetInstanceBaseUrl().c_str();
            network->connectToHostEncrypted(QString(config->GetInstanceBaseUrl().c_str()));
        }
        else
        {
            qDebug() << "HTTP connection to " << config->GetInstanceBaseUrl().c_str();
            network->connectToHost(QString(config->GetInstanceBaseUrl().c_str()));
        }

        if (config->Guest)
        {
            qDebug() << "Guest mode requested.";
            // If guest, try to check for basic status connection.
            GetStatusRequest req;

            if (handleRequest(req))
            {
                state = Guest;
                failCount = 0;
            }
            else
            {
                state = Failing;
                failCount = 0;
            }
        }
        else
        {
            if (!IsCookieValid())
            {
                PostAuthSignInRequest req(GetProvidedUsername(), password);

                if (!handleRequest(req))
                {
                    failCount = 0;
                    qWarning() << "Login failed for username " << GetProvidedUsername().c_str();
                    Logout();
                    return;
                }

                config->Username = GetProvidedUsername();
                config->Password = password;
                config->Session = GetSessionCookie();
            }

            // Test connection with request that requires auth
            GetUserMeRequest userReq;

            connect(&userReq, &GetUserMeRequest::Success, this, [&](const User &) // clazy:skip
            {
                config->Save();

                // connected!
                state = Online;
                failCount = 0;
            });

            if (!handleRequest(userReq))
            {
                state = Failing;
                failCount = 0;
                return;
            }

            // Block until success or failure.
            while (state == Connecting)
            {
                QThread::msleep(500);
            }
        }
    }

    void APIAccess::Perform(APIRequest* request)
    {
        try
        {
            request->Perform(this);
        }
        catch (exception &e)
        {
            request->Fail(e.what(), nullptr);
        }
    }

    bool APIAccess::handleRequest(APIRequest &request)
    {
        try
        {
            request.Perform(this);

            APIRequestCompletionState completionState = request.GetCompletionState();
            if (completionState != Completed)
            {
                if (completionState == Failed)
                {
                    APIRequestFailure &failure = *request.GetFailure();
                    if (!failure.code)
                        handleFailure();
                    else
                        handleWebException(*failure.code);
                }
                return false;
            }

            failCount = 0;
            return true;
        }
        catch (exception &e)
        {
            request.Fail(e.what(), nullptr);
            qCritical() << "Error occurred while handling an API request.";
            return false;
        }
    }

    void APIAccess::handleFailure()
    {
        qWarning() << "API fail count is now " << ++failCount;

        if (failCount >= 3)
        {
            state = Failing;
            flushQueue();
        }
    }

    void APIAccess::handleWebException(HttpCode &code)
    {
        switch (code.err)
        {
            case QNetworkReply::ContentAccessDenied:
            case QNetworkReply::AuthenticationRequiredError:
                Logout();
                break;
            default:
                handleFailure();
                break;
        }
    }

    void APIAccess::Queue(APIRequest &request)
    {
        lock_guard lock(queueLock);

        if (state == Offline)
        {
            request.Fail("User not logged in.", nullptr);
            return;
        }

        queue.push(&request);
    }

    void APIAccess::flushQueue(bool failOldRequests)
    {
        lock_guard lock(queueLock);

        while (!queue.empty())
        {
            if (failOldRequests)
                queue.front()->Fail("Request failed due to flush operation requested.", nullptr);

            queue.front()->deleteLater(); // The popped requests won't probably be owned by anyone in the future

            queue.pop();
        }
    }

    void APIAccess::Logout()
    {
        password = "";

        state = Offline;
        flushQueue();
    }

    void APIAccess::Login(string &username, string &password)
    {
        qDebug() << "Login in with username: " << username.c_str();
        this->username = username;
        this->password = password;
    }

    string APIAccess::GetProvidedUsername() const
    {
        return username;
    }

    APIState APIAccess::GetState() const
    {
        return state;
    }

    APIAccess::~APIAccess()
    {
        loopThread.requestInterruption();
        flushQueue();
        config->Username = username;
        config->Password = password;
        config->Host = host;
        config->Save();
        networkCookieJar->deleteLater();
    }

    void APIAccess::SetHost(const string &host)
    {
        this->host = host;
        config->Host = host;
        config->Save();

        Logout();
    }

    void APIAccess::run()
    {
        qDebug() << "API Loop thread started!";

        while (!isInterruptionRequested)
        {
            qDebug() << "API tick";
            if (state == Failing)
            {
                qWarning() << "APIAccess is in a failing state, waiting a bit before trying again...";
                QThread::sleep(5);
            }

            if (!IsLoggedIn() && !config->Guest)
            {
                state = Offline;
                QThread::msleep(50);
                continue;
            }

            if (state < Guest) // Guest and Online are okay states.
            {
                attemptConnect();

                if (state < Online)
                    continue;
            }

            if (!IsCookieValid())
            {
                Logout();
                continue;
            }

            processQueuedRequests();
            QThread::msleep(50);
        }
    }

    void APIAccess::Stop()
    {
        isInterruptionRequested = true;
    }

    void APIAccess::Start()
    {
        run();
    }
} // mosme