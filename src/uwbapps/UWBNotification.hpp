// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBNOTIFICATION_HPP
#define UWBNOTIFICATION_HPP

#include "hal/uwb_types.hpp"
#include "UWBRangingData.hpp"
#include <Arduino.h>

const int MAX_HANDLERS = 10;

struct HandlerEntry {
    uwb::NotificationType notification_type;
    void (*handler)(void*);
};

class NotificationDispatcher {
public:
    static void RegisterNotification(uwb::NotificationType notification_type, void (*handler)(void*)) {
        for (int i = 0; i < MAX_HANDLERS; ++i) {
            if (handlers[i].handler == nullptr) {
                handlers[i].notification_type = notification_type;
                handlers[i].handler = handler;
                return;
            }
        }
        Serial.println("Handler array is full!");
    }

    static void DispatchNotification(uwb::NotificationType notification_type, void* data) {
        for (int i = 0; i < MAX_HANDLERS; ++i) {
            if (handlers[i].handler != nullptr && handlers[i].notification_type == notification_type) {
                if(notification_type == uwb::NotificationType::RANGING_DATA)
                {   
                    UWBHAL.Log_Array_D("Ranging Data Notification", (uint8_t*)data, sizeof(uwb::RangingResult));
                    uwb::RangingResult result = *(uwb::RangingResult*)data;
                    
                    UWBRangingData rangingData(result);
                    handlers[i].handler(&rangingData);
                } else {
                    handlers[i].handler(data);
                }
                return;
            }
        }
        Serial.print("No handler for notification type: ");
        Serial.println(static_cast<int>(notification_type));
    }

private:
    static HandlerEntry handlers[MAX_HANDLERS];
};

template <uwb::NotificationType NotifType, typename DataType>
class NotificationHandler {
public:
    using CallbackType = void (*)(DataType&);

    static void RegisterCallback(CallbackType callback) {
        NotificationDispatcher::RegisterNotification(NotifType, &HandleNotification);
        getCallback() = callback;
    }

    static void HandleNotification(void* data) {
        if (getCallback()) {
            DataType& ref = *static_cast<DataType*>(data);
            getCallback()(ref);
        }
    }

private:
    static CallbackType& getCallback() {
        static CallbackType callback = nullptr;
        return callback;
    }
};

#endif /* UWBNOTIFICATION_HPP */
