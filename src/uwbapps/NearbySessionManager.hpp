// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl


#ifndef NEARBYSESSIONMANAGER_HPP
#define NEARBYSESSIONMANAGER_HPP

#include <ArduinoBLE.h>
#include "UWBSessionManager.hpp"
#include "NearbySession.hpp"

class NearbySessionManager : public UWBSessionManager_ {
public:
    NearbySessionManager();

    /**
     * @brief setup the callback for when a BLE connection happens
     * 
     * @param connectHandler 
     */
    void onConnect(BLEDeviceEventHandler connectHandler);
    /**
     * @brief setup the callback for when a phone disconnects from BLE
     * 
     * @param disconnectHandler 
     */
    void onDisconnect(BLEDeviceEventHandler disconnectHandler);
    /**
     * @brief callback for when a UWB session starts
     * 
     * @param sessionStopHandler 
     */
    void onSessionStop(BLEDeviceEventHandler sessionStopHandler);

    /**
     * @brief callback for when a UWB session stops
     * 
     * @param sessionStartHandler 
     */
    void onSessionStart(BLEDeviceEventHandler sessionStartHandler);

    /**
     * @brief internal method to handle a situation where the UWB session is being stopped
     * 
     * @param bleDev 
     * @return true 
     * @return false 
     */
    bool handleStopSession(BLEDevice bleDev);

    /**
     * @brief internal method that handles the incoming commands sent by the phone
     * 
     * @param bleDev 
     * @param data 
     */
    void handleTLV(BLEDevice bleDev, uint8_t *data);

    /**
     * @brief start the BLE manager
     * 
     * @param deviceName BLE device name
     */
    void begin(String deviceName);

    /**
     * @brief poll the BLE stack
     * 
     */
    void poll(void);

    /**
     * @brief find a session by BLEDevice
     * 
     * @param dev 
     * @return NearbySession& 
     */
    NearbySession &find(BLEDevice dev) override;

    /**
     * @brief add a session
     * 
     * @param sess 
     * @return true 
     * @return false 
     */
    bool addSession(NearbySession &sess);
    
    /**
     * @brief Get the singleton object
     * 
     * @return NearbySessionManager& 
     */
    static NearbySessionManager& instance();

    

private:
    
    NearbySessionManager(NearbySessionManager const &) = delete;
    void operator=(NearbySessionManager const &) = delete;

    NearbySession emptySession;

    /**
     * @brief callbacks
     * 
     */
    BLEDeviceEventHandler clientConnectionHandler;
    BLEDeviceEventHandler clientDisconnectionHandler;
    BLEDeviceEventHandler sessionStoppedHandler;
    BLEDeviceEventHandler sessionStartedHandler;

    /**
     * @brief BLE services and characteristics according to the spec
     * 
     */
    BLEService nearbyService; //("48FE3E40-0817-4BB2-8633-3073689C2DBA"); // create service
    BLECharacteristic accessoryConfigDataChar; //("95E8D9D5-D8EF-4721-9A4E-807375F53328", BLERead, 128);
    BLEService configService; //("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    BLECharacteristic rxCharacteristic; //("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLEWriteWithoutResponse, 128);
    BLECharacteristic txCharacteristic; //("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify, 128);

    /**
     * @brief inner BLE callbacks
     * 
     * @param central 
     */
    static void blePeripheralConnectHandler(BLEDevice central);
    static void blePeripheralDisconnectHandler(BLEDevice central);
    static void rxCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic);

    bool bleInitialized;
private:
    
};

extern NearbySessionManager &UWBNearbySessionManager;

#endif