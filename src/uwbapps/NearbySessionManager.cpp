// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBAppParamList.hpp"
#include "NearbySessionManager.hpp"

NearbySessionManager::NearbySessionManager() {
   // SEMAPHORE_CREATE();
}

void NearbySessionManager::blePeripheralConnectHandler(BLEDevice central)
{
    // central connected event handler

    NearbySession newSession(central);
    NearbySessionManager::instance().addSession(newSession);
    if (NearbySessionManager::instance().clientConnectionHandler)

        NearbySessionManager::instance().clientConnectionHandler(central);
}

void NearbySessionManager::onConnect(BLEDeviceEventHandler connectHandler)
{
    clientConnectionHandler = connectHandler;
}

void NearbySessionManager::onDisconnect(BLEDeviceEventHandler disconnectHandler)
{
    clientDisconnectionHandler = disconnectHandler;
}

void NearbySessionManager::onSessionStop(BLEDeviceEventHandler sessionStopHandler)
{
    sessionStoppedHandler = sessionStopHandler;
}

void NearbySessionManager::onSessionStart(BLEDeviceEventHandler sessionStartHandler)
{
    sessionStartedHandler = sessionStartHandler;
}

void NearbySessionManager::blePeripheralDisconnectHandler(BLEDevice central)
{
    // central disconnected event handler

    //NearbySessionManager::instance().handleStopSession(central);
    
   

    
    if (NearbySessionManager::instance().clientDisconnectionHandler)
        NearbySessionManager::instance().clientDisconnectionHandler(central);
    
    NearbySessionManager::instance().deleteSession(NearbySessionManager::instance().find(central).sessionID());

}

void NearbySessionManager::rxCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic)
{

    NearbySessionManager::instance().handleTLV(central, (uint8_t *)characteristic.value());
}

bool NearbySessionManager::handleStopSession(BLEDevice bleDev)
{
    bool status = true;
    uwb::Status operation = uwb::Status::SUCCESS;
    NearbySession &nearbySession = NearbySessionManager::instance().find(bleDev);
   // SEMAPHORE_TAKE();
    delay(2000);
    while (nearbySession.sessionState() != notCreated)
    {
        switch (nearbySession.sessionState())
        {
        case notStarted:
            UWBHAL.Log_D("Deleting session: %04X", nearbySession.sessionID());
            nearbySession.stop();
            operation = nearbySession.deInit();

            if (operation == uwb::Status::SUCCESS || operation == uwb::Status::SESSION_NOT_EXIST)
            {
                nearbySession.sessionState(notCreated);
                status = true;
            }
            else
            {
                status = false;
            }
            //delay(2000);
            break;
        case Started:
            UWBHAL.Log_D("Stopping session: %04X", nearbySession.sessionID());
            operation = nearbySession.stop();
            UWBHAL.Log_D("Stopped session with status: %04X", operation);

            if (operation == uwb::Status::SUCCESS || operation == uwb::Status::SESSION_NOT_EXIST)
            {
                nearbySession.sessionState(notStarted);
                status = true;
            }
            else
            {
                status = false;
            }
            break;
            
        default:
            UWBHAL.Log_E("Stop session wrong state: %d", nearbySession.sessionState());
            status = false;
            break;
        }
    }
   // SEMAPHORE_GIVE();
    if (sessionStoppedHandler != nullptr)
        sessionStoppedHandler(bleDev);
    return status;
}

void NearbySessionManager::handleTLV(BLEDevice bleDev, uint8_t *data)
{
    uwb::Status uwb_status = uwb::Status::FAILED;

    uint8_t response;

    if (data == NULL)
    {
        UWBHAL.Log_W("handleTLV data is NULL");
    }
    NearbySession &nearbySession = NearbySessionManager::instance().find(bleDev);

    // TODO check if object is valid

    switch (data[0])
    {
    case kMsg_ConfigureAndStart:
    {
        nearbySession.sessionState(notStarted);
        if (nearbySession.deviceType() == Android)
        {
            if (nearbySession.startAndroid(data) == uwb::Status::SUCCESS)
            {
                response = kRsp_UwbDidStart;
                txCharacteristic.writeValue(&response, sizeof(response));
            }
            else
            {
                UWBHAL.Log_E("Could not start Android Nearby Session");
            }
            {
                response = kRsp_UwbDidStart;
                txCharacteristic.writeValue(&response, sizeof(response));
            }
        }
        else if (nearbySession.deviceType() == iOS)
        {
            /* Fill-in input structure with device role/type and device mac address*/
            UWBHAL.Log_Array_D("shareable data", data,30);

            if (nearbySession.startIOS(data) == uwb::Status::SUCCESS)
            {
                response = kRsp_UwbDidStart;
                txCharacteristic.writeValue(&response, sizeof(response));
                if (nearbySession.shouldUpdateAccessory())
                {
                    const uint8_t tmpData[50] = {0};
                    accessoryConfigDataChar.writeValue(tmpData, 50);//neds to be fixed
                }
            }
            else
            {
                UWBHAL.Log_E("Could not start IOS Nearby Session");
            }
        }
        else
        {
            uwb_status = uwb::Status::FAILED; // Unknown platform detected
            UWBHAL.Log_E("Unknown platform detected");
        }
    }
    break;

    case kMsg_Initialize_iOS:
    {
        /* Start command received
         * Fill the ConfigData and send it over BLE to the phone application
         */

        if (nearbySession.configIOS() == uwb::Status::SUCCESS)
        {
            uint8_t *BLEmessage_iOS = nearbySession.config();
            for (int jj=0; jj < 1 + nearbySession.configLen(); jj++)
            {
                if(BLEmessage_iOS[jj]<0x10)
                    Serial.print("0");
                Serial.print(BLEmessage_iOS[jj], HEX);
            }
            Serial.print("\n");
            if (nearbySession.shouldUpdateAccessory())
            {
                UWBHAL.Log_I(" Following spec: 1.1");
                /* Spec 1.1 required to update GATT server
                Update the GATT server with the same BLEmessage (only removing Response ID that is not part of the original definition) */
                accessoryConfigDataChar.writeValue(BLEmessage_iOS + 1, nearbySession.configLen() - 1);

                /* Need to send the exact data over ble */
                
                txCharacteristic.writeValue(BLEmessage_iOS, nearbySession.configLen());
            }
            else
            {
                UWBHAL.Log_I(" Following spec 1.0");
                /* Spec 1.0 support, clock drift not sent over BLE. BLE message size must  */
                txCharacteristic.writeValue(BLEmessage_iOS, nearbySession.configLen());
            }
        }
    }
    break;

    case kMsg_Initialize_Android:
    {

        if (nearbySession.configAndroid() == uwb::Status::SUCCESS)
        {
            uint8_t *BLEmessage_Android = nearbySession.config();

            /* Need to send the exact data from ConfigData  over ble */
            txCharacteristic.writeValue(BLEmessage_Android, nearbySession.configLen());
        }
        else
            UWBHAL.Log_E("Android config fail");
    }
    break;

    case kMsg_Stop:
        /* Stop command received
         * Stop UWB and send back the response to the phone
         */
        UWBHAL.Log_I("Received stop message");
        if (!NearbySessionManager::instance().handleStopSession(bleDev))
        {
            UWBHAL.Log_E("Stop session failed");
        }
        else
        {
            uwb_status = uwb::Status::SUCCESS;
        }
        response = kRsp_UwbDidStop;
        txCharacteristic.writeValue(&response, sizeof(response));

        break;

    default:
        UWBHAL.Log_W("Unknown command, skipping");
        break;
    }


}
void NearbySessionManager::begin(String deviceName)
{

    BLEService nearbyServ("48FE3E40-0817-4BB2-8633-3073689C2DBA"); // create service
    BLECharacteristic accessoryConfigData("95E8D9D5-D8EF-4721-9A4E-807375F53328", BLERead, 128);
    BLEService configServ("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
    BLECharacteristic rxChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLEWriteWithoutResponse, 128);
    BLECharacteristic txChar("6E400003-B5A3-F393-E0A9-E50E24DCCA9E", BLENotify, 128);
    this->nearbyService = nearbyServ;
    this->accessoryConfigDataChar = accessoryConfigData;
    this->configService = configServ;
    this->rxCharacteristic = rxChar;
    this->txCharacteristic = txChar;

    while (!BLE.begin())
        UWBHAL.Log_E("starting Bluetooth® Low Energy module failed!");
    
    // set the UUID for the service this peripheral advertises
    BLE.setAdvertisedService(configService);
    configService.addCharacteristic(rxCharacteristic);
    configService.addCharacteristic(txCharacteristic);
    BLE.addService(configService);
    nearbyService.addCharacteristic(accessoryConfigDataChar);
    BLE.addService(nearbyService);
    BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
    BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
    rxCharacteristic.setEventHandler(BLEWritten, rxCharacteristicWritten);
    // set the local name peripheral advertises
    BLE.setLocalName(deviceName.c_str());
    BLE.setDeviceName(deviceName.c_str());
}

void NearbySessionManager::poll(void)
{
    if (!bleInitialized)
    {
        BLE.advertise();
        bleInitialized = true;
    }
    BLE.poll();

}

NearbySession &NearbySessionManager::find(BLEDevice dev) 
{
    NearbySession *tempSession;
    for (int i = 0; i < numSessions; i++)
    {

        tempSession = (NearbySession *)sessions[i];

        if (tempSession->bleAddress() == dev.address())
        {
            return *(NearbySession *)sessions[i];
        }
    }
    return emptySession;
}

bool NearbySessionManager::addSession(NearbySession &sess)
{
    NearbySession *newSess = new NearbySession();
    newSess->sessionID(sess.sessionID());
    newSess->sessionType(sess.sessionType());
    newSess->bleDevice(sess.bleDevice());

    if (numSessions >= maxSessions)
        return false;

    sessions[numSessions++] = newSess; //&sess;
    return true;
}

NearbySessionManager &NearbySessionManager::instance()
{
    static NearbySessionManager instance;

    return instance;
}

NearbySessionManager &UWBNearbySessionManager = UWBNearbySessionManager.instance();

