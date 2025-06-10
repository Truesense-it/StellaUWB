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
    //BaseType_t xReturned;
    printer=&printInterface;
    UWBHAL.setLogLevel(logLevel);
    UWBHAL.setPrintCallback(logCB);
   
    //int old = uxTaskPriorityGet(NULL);
    //vTaskPrioritySet(NULL,1);
    initUWB();
    //vTaskPrioritySet(NULL,old);
	
    
}
void UWB_::end(void)
{
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
