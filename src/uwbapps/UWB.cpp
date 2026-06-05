// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "Arduino.h"
#include "SPI.h"
#include "UWB.hpp"

#include "UWBSessionManager.hpp"

/**************************************************************************************
 * NAMESPACE
 **************************************************************************************/


extern "C" int runtime_log_level;

extern "C" void logCB(const char *str)
{
   UWB_::printMessage(str);
}

extern "C" void SystemCallback(uwb::NotificationType opType, void *pData)
{
    NotificationDispatcher::DispatchNotification(opType, pData);  
}


#ifdef EARLY_AUTOSTART_FREERTOS
EARLY_AUTOSTART_FREERTOS
#endif

HandlerEntry NotificationDispatcher::handlers[MAX_HANDLERS] = {};
Print* UWB_::printer = nullptr; 


UWB_::UWB_()
{
    
}

void UWB_::begin(Print& printInterface, uwb::LogLevel logLevel)
{
    printer=&printInterface;
    UWBHAL.setLogLevel(logLevel);
    UWBHAL.setPrintCallback(logCB);

    // Multi-session Nearby: skip full re-init when HAL is already running
    if (halKeepAlive) {
        uwb::DeviceState devState = uwb::DeviceState::NOT_INITIALIZED;
        if (UWBHAL.getDeviceState(devState) == uwb::Status::SUCCESS &&
            devState != uwb::DeviceState::NOT_INITIALIZED &&
            devState != uwb::DeviceState::UNKNOWN) {
            UWBHAL.Log_D("begin: HAL already up (keepAlive), skipping initialize");
            return;
        }
    }

    initUWB();
}

void UWB_::end(void)
{
    // Multi-session Nearby: avoid full HAL shutdown between BLE sessions
    if (halKeepAlive) {
        UWBHAL.Log_I("UWB.end() skipped (keepAlive mode)");
        return;
    }

    UWBHAL.deinitialize();
    delay(100); // Wait for the deinitialization to complete
    if (UWBHAL.shutdown() != uwb::Status::SUCCESS) {
        UWBHAL.Log_E("ShutDown Failed");
    }
}




uint8_t UWB_::initUWB(void)
{
    uwb::Status status;
    uwb::DeviceInfo devInfo;
	
    // Initialize the UWB stack
    status=UWBHAL.initialize(&SystemCallback);

    if (status != uwb::Status::SUCCESS) {
        UWBHAL.Log_E("Init Failed");
        return status;
    }
    
    UWBHAL.Log_D("init done");
    // status = UWBHAL.getDeviceInfo(devInfo);
    
    // //printDeviceInfo(&devInfo);
    // if (status != uwb::Status::SUCCESS) {
    //     UWBHAL.Log_E("GetDeviceInfo() Failed");
    //     return status;
    // }

    return (uint8_t) status;
    
}

void UWB_::deInitUWB(void)
{
    uwb::Status status;
    if (UWBHAL.shutdown() != uwb::Status::SUCCESS) {
        UWBHAL.Log_E("ShutDown Failed");
    }

    if (status == uwb::Status::TIMEOUT ) {
       // UWBHAL.HWreset(); //TBD
       
    }
} 

uint8_t UWB_::state()
{
	uwb::DeviceState state;
    return (uint8_t) UWBHAL.getDeviceState(state);
}


void UWB_::printMessage(const char *message)
{
     if (printer) {
            printer->print(message);
        } 
}

UWB_ &UWB_::getInstance()
{
    static UWB_ instance;

    return instance;
}

UWB_ &UWB = UWB.getInstance();
