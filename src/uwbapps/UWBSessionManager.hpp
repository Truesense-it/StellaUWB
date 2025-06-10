// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBSESSIONMANAGER_HPP
#define UWBSESSIONMANAGER_HPP

#include "UWBSession.hpp"
#include <ArduinoBLE.h>
#include "NearbySession.hpp"
/**
 * @brief Utility class to keep a list of sessions
 * 
 */
class UWBSessionManager_ {
public:
    /**
     * @brief Construct a new UWBSessionManager_ object
     * 
     */
    UWBSessionManager_();
    /**
     * @brief deletea session matching a specific session ID
     * 
     * @param sessionID 
     * @return true 
     * @return false 
     */
    bool deleteSession(uint32_t sessionID);

    /**
     * @brief add a session to the list
     * 
     * @param sess 
     * @return true 
     * @return false 
     */
    bool addSession(UWBSession& sess);

    /**
     * @brief Get the Session By ID 
     * 
     * @param id 
     * @return UWBSession& 
     */
    UWBSession& getSessionByID(uint32_t id);

    // /**
    //  * @brief Get the Session By Handle object
    //  * 
    //  * @param sessionHandle 
    //  * @return UWBSession& 
    //  */
    // UWBSession& getSessionByHandle(uint32_t sessionHandle);

    /**
     * @brief clean the list
     * 
     */
    void deleteAllSessions();

    /**
     * @brief starts all sessions in the list
     * 
     * @return tUWBAPI_STATUS 
     */
    uwb::Status startSessions();

    /**
     * @brief stop all sessions in the list
     * 
     * @return tUWBAPI_STATUS 
     */
    uwb::Status stopSessions();

    /**
     * @brief utility method implemented in the NerabySessionManager
     * 
     * @param dev 
     * @return NearbySession& 
     */
    virtual NearbySession& find(BLEDevice dev) {(void)dev; };
    static UWBSessionManager_& getInstance();

private:
    bool isIDInUse(uint32_t id);
    
    UWBSessionManager_(UWBSessionManager_ const &) = delete;
    void operator=(UWBSessionManager_ const &) = delete;

protected:
    static const int maxSessions = 5;
    int numSessions;
    UWBSession* sessions[maxSessions];
    UWBSession emptySession;
};


extern UWBSessionManager_ &UWBSessionManager;

#endif //_UWBSESSIONMANAGER_H_
