// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBSessionManager.hpp"


UWBSessionManager_::UWBSessionManager_() {}

bool UWBSessionManager_::deleteSession(uint32_t sessionID)
{
    for (int i = 0; i < numSessions; ++i)
    {
        if (sessions[i]->sessionID() == sessionID)
        {
            delete sessions[i];
            // Shift elements to fill the gap
            for (int j = i; j < numSessions - 1; ++j) {
                sessions[j] = sessions[j + 1];
            }
            numSessions--;
            sessions[numSessions]=nullptr;
            return true;
        }
    }
    return false;
}
bool UWBSessionManager_::addSession(UWBSession& sess)
{
    UWBSession *newSess= new UWBSession();
    newSess->sessionID(sess.sessionID());
    newSess->sessionType(sess.sessionType());
    
    if(numSessions >= maxSessions)
        return false;

    sessions[numSessions++]= newSess;//&sess;
    return true;
    
}

UWBSession& UWBSessionManager_::getSessionByID(uint32_t id)
{
    for (int i = 0; i < numSessions; ++i)
    {
        if (sessions[i]->sessionID() == id)
        {
            return *sessions[i];
        }
    }
    return emptySession; // Session ID not found
}

// UWBSession& UWBSessionManager_::getSessionByHandle(uint32_t sessionHandle)
// {
//     for (int i = 0; i < numSessions; ++i)
//     {
//         if ( sessions[i]->sessionHandle() == sessionHandle)
//         {
//             return *sessions[i];
//         }
//     }
//     return emptySession; // Session not found
// }

void UWBSessionManager_::deleteAllSessions()
{
    for (int i = 0; i < numSessions; ++i)
    {
        
        sessions[i]=nullptr;
    }
    numSessions=0;
}

uwb::Status UWBSessionManager_::startSessions()
{
    uwb::Status status=uwb::Status::SUCCESS;
    uwb::Status tmpStatus;
    
    for (int i = 0; i < numSessions; ++i)
    {
        
        tmpStatus = sessions[i]->start();
        if (tmpStatus != uwb::Status::SUCCESS)
            status=tmpStatus;
    }
    return status;
}

uwb::Status UWBSessionManager_::stopSessions()
{
    uwb::Status status=uwb::Status::SUCCESS;
    uwb::Status tmpStatus;
    
    for (int i = 0; i < numSessions; ++i)
    {
        tmpStatus = sessions[i]->stop();
        if (tmpStatus != uwb::Status::SUCCESS)
            status=tmpStatus;
    }
    return status;
}

bool UWBSessionManager_::isIDInUse(uint32_t id)
{
    for (int i = 0; i < numSessions; ++i)
    {
        if (sessions[i]->sessionID() == id)
        {
            return true;
        }
    }
    return false;
}

UWBSessionManager_ &UWBSessionManager_::getInstance()
{
    static UWBSessionManager_ instance;

    return instance;
}

UWBSessionManager_ &UWBSessionManager = UWBSessionManager.getInstance();
