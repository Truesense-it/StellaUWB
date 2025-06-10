// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWB_HPP
#define UWB_HPP

#include "hal/uwb_hal.hpp"
#include "UWBSessionManager.hpp"
#include "NearbySessionManager.hpp"
#include "UWBNotification.hpp"
#include "UWBRangingData.hpp"
#include "Arduino.h"


#define CHECK(f, rv)                   \
    if (0 != rv)                       \
    {                                  \
        UWBHAL.Log_E(f, ": %d\n", rv); \
        return rv;                     \
    }


/** Q-format : Q(x.y)
 * Q(x.y) : where x is number of integer bits + 1 (sign bit)
 * And y is number of fractional bits
*/
#define TO_Q_8_8(X) ((X) >> 8), ((X)&0xFF)

/** Here for integer bits x : x >> 0x0B
 *  Now for fractional bits y : y & 0x3FF
 *  where 2^11 = 2048 : which ≈ 0-2047(0x7FF in hex)
*/
#define TO_Q_5_11(X) ((X) >> 0x0B), ((X)&0x7FF)

/** Here for integer bits x : x >> 0x0A
 *  Now for fractional bits y : y & 0x3FF
 *  where 2^10 = 1024 : which ≈ 0-1023(0x3FF in hex)
*/
#define TO_Q_6_10(X) ((X) >> 0x0A), ((X)&0x3FF)

#define TO_Q_9_7(X) ((X) >> 7), ((X)&0x7F)

typedef void (*RangingCallbackType)(UWBRangingData&);
typedef void (*SessionInfoCallbackType)(uwb::SessionInfo&);
typedef void (*DataTxCallbackType)(uwb::DataTransmit&);
typedef void (*DataRxCallbackType)(uwb::DataPacket&);
typedef void (*ErrorCallbackType)(uwb::GenericError&);




/**
 * Main class for UWB interface. Inits the HW, retrieves state, 
 * allows for sending custome device configuration 
 * @brief main class for UWB library
 * 
 */
class UWB_
{

public:

    /**
     * @brief Construct a new UWB object
     * 
     */   
    UWB_();

   /**
    * @brief setup and start the UWB stack
    * 
    * @param printInterface Stream-based interface where logs are printed
    * @param logLevel the log level
    */
    void begin(Print& printInterface = Serial, uwb::LogLevel logLevel = uwb::LogLevel::UWB_INFO_LEVEL);

    /**
     * @brief stop the UWB engine
     * 
     */
    void end(void);
    /**
     * @brief init the UWB engine (automatically called by begin())
     * 
     * @return uint8_t 
     */
    uint8_t initUWB(void);
    /**
     * @brief deinit the UWB engine
     * 
     */
    void deInitUWB(void);
       
    /**
     * @brief Get the Sdevice state
     * 
     * @return #uwb::Status::SUCCESS               on success
     * @return #uwb::Status::INVALID_PARAM    if parameter is invalid
     * @return #uwb::Status::NOT_INITIALIZED  if UWB stack is not initialized
     * @return #uwb::Status::FAILED           otherwise
     * @return #uwb::Status::TIMEOUT          if command is timeout
     */
    uint8_t state();
    
    /**
     * @brief register a callback for when ranging data is notified from the UWB stack
     * 
     * @param callback 
     */
    void registerRangingCallback(RangingCallbackType callback)
    {
        NotificationHandler<uwb::NotificationType::RANGING_DATA, UWBRangingData>::RegisterCallback(callback);
    };

    /**
     * @brief registers a callback for when a session information notification arrives
     * 
     * @param callback 
     */
    void registerSessionInfoCallback(SessionInfoCallbackType callback)
    {
        NotificationHandler<uwb::NotificationType::SESSION_DATA, uwb::SessionInfo>::RegisterCallback(callback);
    };

    /**
     * @brief register the callback that notifies the status of a data TX 
     * 
     * @param callback 
     */
    void registerDataTxCallback(DataTxCallbackType callback)
    {
        NotificationHandler<uwb::NotificationType::DATA_TRANSMIT_NTF, uwb::DataTransmit>::RegisterCallback(callback);
    };
    
    /**
     * @brief callback for notification of data RX
     * 
     * @param callback 
     */
    void registerDataRxCallback(DataRxCallbackType callback)
    {
        NotificationHandler<uwb::NotificationType::DATA_RCV_NTF  , uwb::DataPacket>::RegisterCallback(callback);
    };


    static void printMessage(const char* message);

    static UWB_& getInstance();

private:
    UWB_(UWB_ const &) = delete;
    void operator=(UWB_ const &) = delete;
    static Print* printer;
};

extern UWB_ &UWB;

#endif /* UWB */
