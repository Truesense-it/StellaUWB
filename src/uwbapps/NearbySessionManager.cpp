// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "Arduino.h"
#include "UWBAppParamList.hpp"
#include "NearbySessionManager.hpp"
#include "NearbyBlePeerNotify.hpp"
#include "UWB.hpp"
#include <rtos/Mutex.h> // session list guard (forward-declared in the header)

NearbySessionManager::NearbySessionManager()
    : bleInitialized(false), txValueHandle(0), cleanupInProgress(false), advertisingResumeAt(0),
      sessionMutex(nullptr) {
   numSessions = 0;
   // sessionMutex is deliberately not created here: this constructor runs during
   // static initialisation, before the RTOS scheduler starts. It is created in
   // begin(); until then lockSessions()/unlockSessions() are no-ops.
}

// Checked acquire. Returns true if the caller now holds the lock and must call
// unlockSessions(), false if it does not. A null mutex means the pre-begin()
// single-threaded phase, where there is nothing to guard.
bool NearbySessionManager::lockSessions()
{
    if (sessionMutex == nullptr) {
        return false;
    }
    if (!sessionMutex->trylock_for(kSessionLockTimeoutMs)) {
        UWBHAL.Log_E("session list lock timeout (%lums) - proceeding unguarded",
                     (unsigned long)kSessionLockTimeoutMs);
        return false;
    }
    return true;
}

// Non-blocking acquire for the UWB HAL thread. The contention window is
// microseconds (walking a <=5 element array), so this should never fail, but the
// HAL thread must never block here. On failure the caller skips attribution for
// that one ranging sample.
bool NearbySessionManager::tryLockSessions()
{
    if (sessionMutex == nullptr) {
        return false;
    }
    return sessionMutex->trylock();
}

void NearbySessionManager::unlockSessions()
{
    if (sessionMutex != nullptr) {
        sessionMutex->unlock();
    }
}

// Route TX notify to the requesting peer when multiple phones are connected.
bool NearbySessionManager::notifyTx(BLEDevice peer, const uint8_t *data, int length)
{
    if (txValueHandle == 0) {
        txValueHandle = nearbyBleValueHandle(txCharacteristic);
    }

    bool ok;
    if (numSessions > 1) {
        ok = nearbyBleNotifyPeer(peer, txValueHandle, data, length);
    } else {
        ok = txCharacteristic.writeValue(data, length) > 0;
    }
    // Make a dropped notification visible instead of failing silently:
    // nearbyBleNotifyPeer refuses a payload larger than MTU-3.
    if (!ok) {
        UWBHAL.Log_E("notifyTx FAILED peer=%s len=%d numSessions=%d",
                     peer.address().c_str(), length, numSessions);
    }
    return ok;
}

void NearbySessionManager::restartAdvertising()
{
    // ArduinoBLE stops advertising on connect, so it must be restarted
    // explicitly to accept further peers. Skipped while a teardown is settling.
    if (cleanupInProgress) {
        return;
    }
    BLE.setConnectable(true);
    BLE.advertise();
}

void NearbySessionManager::pauseAdvertisingForCleanup(unsigned long settleMs)
{
    cleanupInProgress = true;
    advertisingResumeAt = millis() + settleMs;
    BLE.setConnectable(false);
    BLE.stopAdvertise();
    UWBHAL.Log_D("BLE advertising paused for UWB cleanup (%lu ms)", settleMs);
}

void NearbySessionManager::resumeAdvertisingIfReady()
{
    if (!cleanupInProgress) {
        return;
    }
    if ((long)(millis() - advertisingResumeAt) < 0) {
        return;
    }

    cleanupInProgress = false;
    if (numSessions < maxSessions) {
        BLE.setConnectable(true);
        BLE.advertise();
        UWBHAL.Log_D("BLE advertising resumed after cleanup settle");
    }
}

void NearbySessionManager::notifySessionStarted(BLEDevice dev)
{
    if (sessionStartedHandler != nullptr) {
        sessionStartedHandler(dev);
    }
}

void NearbySessionManager::blePeripheralConnectHandler(BLEDevice central)
{
    // central connected event handler

    NearbySessionManager &mgr = NearbySessionManager::instance();

    if (mgr.cleanupInProgress) {
        UWBHAL.Log_W("Rejecting BLE connect during UWB cleanup settle");
        central.disconnect();
        return;
    }

    NearbySession newSession(central);
    if (!mgr.addSession(newSession)) {
        UWBHAL.Log_W("addSession failed (duplicate or max sessions)");
        return;
    }

    if (mgr.clientConnectionHandler)
        mgr.clientConnectionHandler(central);

    // Restart advertising so additional phones can connect
    if (mgr.numSessions < maxSessions) {
        mgr.restartAdvertising();
    }
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

    NearbySessionManager &mgr = NearbySessionManager::instance();
    NearbySession &sess = mgr.find(central);

    // Pause new connections while the UWB session for this peer is torn down.
    // The resume deadline is (re)armed after teardown below.
    mgr.pauseAdvertisingForCleanup(kReconnectSettleMs);

    if (sess.sessionState() != notCreated) {
        mgr.handleStopSession(central);
    }

    mgr.deleteSessionByDevice(central);

    // Arm the settle window now that teardown has completed, so a slow or retried
    // teardown does not consume it. handleStopSession() is guaranteed to return,
    // so this always runs, cleanupInProgress is always cleared later by
    // resumeAdvertisingIfReady(), and advertising is never left off.
    mgr.advertisingResumeAt = millis() + kReconnectSettleMs;

    if (mgr.clientDisconnectionHandler)
        mgr.clientDisconnectionHandler(central);
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
    // Each teardown state gets its own wall-clock deadline, re-armed whenever the
    // state advances, so a slow but successful stop() cannot consume the budget
    // deInit() needs - which would stop the session without deinitialising it and
    // leak the firmware slot. Every state is therefore guaranteed at least one
    // attempt. The per-state attempt cap covers the case where the HAL returns
    // immediately. Whichever limit trips first forces notCreated so the loop exits.
    //
    // kStopBudgetMs is not a fine-grained tuning knob: UWB_CMD_TIMEOUT is 2000 ms,
    // so any value below that means "allow one blocking attempt per state, then
    // bail". It is checked between UCI calls and cannot interrupt one in flight.
    const unsigned long kStopBudgetMs = 400;
    const int kMaxStopRetries = 3;
    unsigned long stateDeadline = millis() + kStopBudgetMs;
    int attempts = 0;
    // True once the Started branch below has stopped this session in this call,
    // which makes the stop() in the notStarted branch redundant. Every UCI command
    // issued while another session is streaming is a chance to hit the HAL
    // SPI-write fault, so it is skipped in that case only.
    bool alreadyStopped = false;
    const unsigned long fieldStartMs = millis();
    // Deliberately not locked: this loop blocks on UCI for seconds.
    while (nearbySession.sessionState() != notCreated)
    {
        // Primary guard: per-state deadline, checked before issuing another UCI command.
        if ((long)(millis() - stateDeadline) >= 0)
        {
            UWBHAL.Log_E("teardown state budget %lums exceeded state=%d lastStatus=%d, forcing notCreated",
                         kStopBudgetMs, nearbySession.sessionState(), (int)operation);
            nearbySession.sessionState(notCreated);
            status = false;
            break;
        }

        switch (nearbySession.sessionState())
        {
        case notStarted:
            UWBHAL.Log_D("Deleting session: %04X", nearbySession.sessionID());
            // Only stop() when this call has not already done so. Entering
            // handleStopSession directly at notStarted is reachable: handleTLV sets
            // notStarted before startIOS/startAndroid, which can then fail. The HAL
            // headers do not document whether sessionDeinit is valid without a
            // preceding stop, so that path keeps the call and logs its status.
            if (!alreadyStopped)
            {
                uwb::Status preStop = nearbySession.stop();
                UWBHAL.Log_D("pre-deInit stop status=%d", (int)preStop);
            }
            operation = nearbySession.deInit();
            UWBHAL.Log_D("deInit status=%d", (int)operation);

            if (operation == uwb::Status::SUCCESS || operation == uwb::Status::SESSION_NOT_EXIST)
            {
                // Advances to notCreated -> loop exits; no re-arm needed.
                nearbySession.sessionState(notCreated);
                status = true;
                attempts = 0;
            }
            else if (++attempts >= kMaxStopRetries)
            {
                UWBHAL.Log_E("deInit attempt cap (%d) hit state=notStarted lastStatus=%d, forcing notCreated",
                             kMaxStopRetries, (int)operation);
                nearbySession.sessionState(notCreated);
                status = false;
            }
            else
            {
                status = false;
                delay(50); // let a transient SPI fault clear; avoid flooding the bus
            }
            break;
        case Started:
            UWBHAL.Log_D("Stopping session: %04X", nearbySession.sessionID());
            operation = nearbySession.stop();
            UWBHAL.Log_D("Stopped session with status: %d", (int)operation);

            if (operation == uwb::Status::SUCCESS || operation == uwb::Status::SESSION_NOT_EXIST)
            {
                nearbySession.sessionState(notStarted);
                status = true;
                attempts = 0;
                // Set on SUCCESS and on SESSION_NOT_EXIST: both mean the session
                // is no longer running, so the stop() in the notStarted branch
                // would be a pointless third UCI command.
                alreadyStopped = true;
                stateDeadline = millis() + kStopBudgetMs; // re-arm: notStarted/deInit gets its own budget
            }
            else if (++attempts >= kMaxStopRetries)
            {
                UWBHAL.Log_E("stop attempt cap (%d) hit state=Started lastStatus=%d, forcing notCreated",
                             kMaxStopRetries, (int)operation);
                nearbySession.sessionState(notCreated);
                status = false;
            }
            else
            {
                status = false;
                delay(50); // let a transient SPI fault clear; avoid flooding the bus
            }
            break;

        default:
            UWBHAL.Log_E("Stop session wrong state: %d, forcing notCreated", nearbySession.sessionState());
            status = false;
            nearbySession.sessionState(notCreated);
            break;
        }
    }
    UWBHAL.Log_I("[FIELD] handleStopSession elapsed=%lums finalStatus=%d",
                 millis() - fieldStartMs, status ? 1 : 0);
    // If the teardown did not complete cleanly, schedule a UWB stack reset. This
    // is non-blocking: scheduleSpiRecovery() sets a flag and the reset runs from
    // UWB.pollRecovery() in loop(). Note that the reset applies to the whole
    // stack, so any other connected phone must start a new session.
    if (!status)
    {
        UWBHAL.Log_W("teardown failed; scheduling deferred SPI/HAL recovery");
        UWB.scheduleSpiRecovery();
    }
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
        return; // do not fall through to switch (data[0])
    }
    NearbySession &nearbySession = NearbySessionManager::instance().find(bleDev);

    // find() returns the shared emptySession on a miss, whose address is
    // 00:00:00:00:00:00. Reject TLVs from a peer with no tracked session, so a
    // still-connected peer cannot drive a real firmware session on emptySession
    // that is never tracked and can never be stopped.
    if (nearbySession.bleAddress() != bleDev.address())
    {
        UWBHAL.Log_W("handleTLV: no session for peer, ignoring TLV");
        return;
    }

    switch (data[0])
    {
    case kMsg_ConfigureAndStart:
    {
        if (nearbySession.sessionState() == Started) {
            response = kRsp_UwbDidStart;
            notifyTx(bleDev, &response, sizeof(response));
            break;
        }

        nearbySession.sessionState(notStarted);
        if (nearbySession.deviceType() == Android)
        {
            if (nearbySession.startAndroid(data) == uwb::Status::SUCCESS)
            {
                response = kRsp_UwbDidStart;
                notifyTx(bleDev, &response, sizeof(response));
                notifySessionStarted(bleDev);
            }
            else
            {
                UWBHAL.Log_E("Could not start Android Nearby Session");
            }
        }
        else if (nearbySession.deviceType() == iOS)
        {
            /* Fill-in input structure with device role/type and device mac address*/
            UWBHAL.Log_Array_D("shareable data", data,30);

            if (nearbySession.startIOS(data) == uwb::Status::SUCCESS)
            {
                response = kRsp_UwbDidStart;
                notifyTx(bleDev, &response, sizeof(response));
                notifySessionStarted(bleDev);
                if (nearbySession.shouldUpdateAccessory() && numSessions <= 1)
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

        uint8_t cfgStatus = nearbySession.configIOS();
        if (cfgStatus == (uint8_t)uwb::Status::SUCCESS)
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
                 * Update the GATT server with the same BLEmessage (only removing Response ID that is not part of the original definition) */
                // accessoryConfigDataChar is a single shared GATT value, so it
                // is written only when at most one session is active. With two or
                // more phones, a concurrent write would let one phone read
                // another's configuration.
                if (numSessions <= 1) {
                    accessoryConfigDataChar.writeValue(BLEmessage_iOS + 1, nearbySession.configLen() - 1);
                } else {
                    UWBHAL.Log_W("Skipping shared accessoryConfigData write (numSessions=%d, last-writer-wins hazard)", numSessions);
                }

                /* Need to send the exact data over ble */
                notifyTx(bleDev, BLEmessage_iOS, nearbySession.configLen());
            }
            else
            {
                UWBHAL.Log_I(" Following spec 1.0");
                /* Spec 1.0 support, clock drift not sent over BLE. BLE message size must */
                notifyTx(bleDev, BLEmessage_iOS, nearbySession.configLen());
            }
        }
        else
        {
            UWBHAL.Log_E("configIOS failed during Initialize_iOS");
        }
    }
    break;

    case kMsg_Initialize_Android:
    {

        if (nearbySession.configAndroid() == uwb::Status::SUCCESS)
        {
            uint8_t *BLEmessage_Android = nearbySession.config();

            /* Need to send the exact data from ConfigData over ble */
            notifyTx(bleDev, BLEmessage_Android, nearbySession.configLen());
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
        notifyTx(bleDev, &response, sizeof(response));

        break;

    default:
        UWBHAL.Log_W("Unknown command, skipping");
        break;
    }

}
void NearbySessionManager::begin(const char* deviceName)
{
    // Create the session-list mutex here, where the scheduler is running, rather
    // than in the constructor, which runs during static initialisation.
    if (sessionMutex == nullptr) {
        sessionMutex = new rtos::Mutex();
    }

    // Multi-session Nearby: keep UWB HAL alive across BLE connect/disconnect cycles
    // (set STELLA_KEEPALIVE=0 to disable)
    UWB.setKeepAlive(STELLA_KEEPALIVE);

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
    BLE.setLocalName(deviceName);
    BLE.setDeviceName(deviceName);
    txValueHandle = nearbyBleValueHandle(txCharacteristic);
}

void NearbySessionManager::poll(void)
{
    if (!bleInitialized)
    {
        BLE.advertise();
        bleInitialized = true;
    }

    UWB.pollRecovery();
    resumeAdvertisingIfReady();
    BLE.poll();

}

NearbySession &NearbySessionManager::find(BLEDevice dev)
{
    NearbySession *tempSession;
    NearbySession *found = nullptr;
    bool locked = lockSessions();
    for (int i = 0; i < numSessions; i++)
    {

        tempSession = (NearbySession *)sessions[i];

        if (tempSession->bleAddress() == dev.address())
        {
            found = (NearbySession *)sessions[i];
            break;
        }
    }
    if (locked) {
        unlockSessions();
    }
    if (found != nullptr) {
        return *found;
    }
    return emptySession;
}

bool NearbySessionManager::addSession(NearbySession &sess)
{
    bool locked = lockSessions();
    if (numSessions >= maxSessions) {
        if (locked) unlockSessions();
        return false;
    }

    for (int i = 0; i < numSessions; i++) {
        NearbySession *existing = (NearbySession *)sessions[i];
        if (existing->bleAddress() == sess.bleAddress()) {
            if (locked) unlockSessions();
            return false;
        }
    }

    NearbySession *newSess = new NearbySession();
    newSess->sessionID(sess.sessionID());
    newSess->sessionType(sess.sessionType());
    newSess->bleDevice(sess.bleDevice());

    sessions[numSessions++] = newSess; //&sess;
    if (locked) {
        unlockSessions();
    }
    return true;
}

// Remove session by BLE address (session ID may be 0 before ConfigureAndStart).
bool NearbySessionManager::deleteSessionByDevice(BLEDevice dev)
{
    String addr = dev.address();
    bool locked = lockSessions(); // guards the delete and array shift
    for (int i = 0; i < numSessions; ++i)
    {
        NearbySession *s = (NearbySession *)sessions[i];
        if (s->bleAddress() == addr)
        {
            // Unlink FIRST, then delete, so no other thread can observe a
            // dangling pointer in sessions[] even briefly.
            for (int j = i; j < numSessions - 1; ++j) {
                sessions[j] = sessions[j + 1];
            }
            numSessions--;
            sessions[numSessions] = nullptr;
            delete s;
            if (locked) {
                unlockSessions();
            }
            return true;
        }
    }
    if (locked) {
        unlockSessions();
    }
    return false;
}

NearbySession *NearbySessionManager::findBySessionHandle(uint32_t sessionHandle)
{
    // Called from the UWB ranging thread; guards the traversal against a
    // concurrent deleteSessionByDevice() on the BLE/loop thread. The acquire is
    // non-blocking so the HAL thread is never held up: if the list is genuinely
    // contended, attribution is skipped for this one sample.
    NearbySession *found = nullptr;
    bool locked = tryLockSessions();
    if (!locked && sessionMutex != nullptr) {
        UWBHAL.Log_W("findBySessionHandle: list busy, skipping attribution for this sample");
        return nullptr;
    }
    // Here: either we hold the lock, or the mutex does not exist yet (pre-begin(),
    // single-threaded phase) in which case there is nothing to guard against.
    for (int i = 0; i < numSessions; i++) {
        NearbySession *s = (NearbySession *)sessions[i];
        if (s->sessionID() == sessionHandle) {
            found = s;
            break;
        }
    }
    if (locked) {
        unlockSessions();
    }
    return found;
}

NearbySessionManager &NearbySessionManager::instance()
{
    static NearbySessionManager instance;

    return instance;
}

NearbySessionManager &UWBNearbySessionManager = UWBNearbySessionManager.instance();
