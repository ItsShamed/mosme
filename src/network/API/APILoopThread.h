//
// Created by Ryuki
//

#pragma once

#include <QObject>
#include <QMutex>
#include <QMutexLocker>

namespace mosme
{

    class APIAccess;
    class APILoopThread : public QThread
    {
        Q_OBJECT
        friend class APIAccess;
        
    private:
        APIAccess* $this;
        QMutex attemptConnectLock;
        QMutex processQueuedRequestsLock;
        
        void attemptConnect();
        void processQueuedRequests();
        
        void run();
        
    signals:
        void callRequest_attemptConnect();
        void callRequest_processQueuedRequests();
        
    public:
        APILoopThread() = delete;
        APILoopThread(APIAccess*);
    };
} // mosme
