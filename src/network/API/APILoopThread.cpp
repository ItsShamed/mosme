//
// Created by Ryuki
//

#include <QThreadPool>
#include <QDebug>
#include <QNetworkCookieJar>
#include "APILoopThread.h"
#include "APIAccess.h"

namespace mosme
{
    APILoopThread::APILoopThread(APIAccess* api) : $this(api)
    {
        qDebug() << "Starting API loop thread...";
    }

    void APILoopThread::attemptConnect()
    {
        qDebug() << "Attempting to call attemptConnect from loop thread";
        emit callRequest_attemptConnect();

        // Little delay in case the Mutex is not directly locked on the main thread
        QThread::msleep(50);
        QMutexLocker lock(&attemptConnectLock);
    }

    void APILoopThread::processQueuedRequests()
    {
        qDebug() << "Attempting to call processQueuedRequests from loop thread";
        emit callRequest_processQueuedRequests();

        // Little delay in case the Mutex is not directly locked on the main thread
        QThread::msleep(50);
        QMutexLocker lock(&processQueuedRequestsLock);
    }

    void APILoopThread::run()
    {
        qDebug() << "API Loop thread started!";


        if ($this->config->PersistentStorage)
        {
            $this->useCredentials = true;
            QNetworkCookie* cookie = $this->config->GetSessionCookie();
            if (cookie)
                $this->networkCookieJar->insertCookie(*cookie);
        }

        $this->network->setCookieJar($this->networkCookieJar);


        if ($this->config->UseHttps)
        {
            qDebug() << "HTTPS connection to " << $this->config->GetInstanceBaseUrl().c_str();
            $this->network->connectToHostEncrypted(QString($this->config->GetInstanceBaseUrl().c_str()));
        }
        else
        {
            qDebug() << "HTTP connection to " << $this->config->GetInstanceBaseUrl().c_str();
            $this->network->connectToHost(QString($this->config->GetInstanceBaseUrl().c_str()));
        }

        while (!isInterruptionRequested())
        {
            if ($this->state == Failing)
            {
                qWarning() << "APIAccess is in a failing state, waiting a bit before trying again...";
                QThread::sleep(5);
            }

            if (!$this->IsLoggedIn() && !$this->config->Guest)
            {
                $this->state = Offline;
                QThread::msleep(50);
                continue;
            }

            if ($this->state < Guest) // Guest and Online are okay states.
            {
                $this->attemptConnect();

                if ($this->state < Online)
                    continue;
            }

            if (!$this->IsCookieValid())
            {
                $this->Logout();
                continue;
            }

            $this->processQueuedRequests();
            QThread::msleep(50);
        }
    }

} // mosme