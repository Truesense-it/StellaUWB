/**
 * Nearby Interaction with 3rd Party Devices from Apple
 * (see https://developer.apple.com/nearby-interaction/)
 *
 * The implementation also works with UWB-enabled Android devices by using different
 * command IDs
 *
 * The device and the mobile phone will need to setup a BLE connection before the
 * actual UWB ranging can start.
 *
 * The BLE session is used to share the configuration parameters necessary to
 * setup the UWB ranging session
 *
 * Multi-session: up to 5 phones may connect. Connect phone B while phone A is
 * still connected to range with both simultaneously. NearbySessionManager handles
 * per-peer BLE notify, per-session cleanup on disconnect, and deferred BLE
 * re-advertising. Do not call UWB.end() on disconnect.
 *
 * Each ranging line includes sessionHandle, UWB peer MAC, and BLE address when known.
 *
 * Build switches (defined in src/uwbapps/NearbySession.hpp):
 *
 *   STELLA_KEEPALIVE  1  Keep the UWB stack initialised across BLE connect and
 *                        disconnect.
 *   STELLA_WATCHDOG   1  Arm the mbed hardware watchdog, so the board resets if
 *                        loop() stops responding.
 *
 * Diagnostic output is controlled by the UWB log level passed to UWB.begin().
 *
 * Examples of UWB-enabled apps working with this demo:
 *
 * NXP Trimensions AR (https://apps.apple.com/us/app/nxp-trimensions-ar/id1606143205)
 * Qorvo Nearby Interaction (https://apps.apple.com/us/app/qorvo-nearby-interaction/id1615369084)
 * NXP android demo (source code https://github.com/nxp-uwb/UWBJetpackExample)
 * Truesense Android demo (source code https://github.com/Truesense-it/TSUwbDemo-Android)
 *
 */

#include <ArduinoBLE.h>
#include <StellaUWB.h>


#if STELLA_WATCHDOG
// Resets the board if loop() stops responding for longer than the timeout.
// Recommended for unattended installations. Set STELLA_WATCHDOG 0 when
// debugging, so a reset does not discard the state you are inspecting.
#include "drivers/Watchdog.h"
static uint32_t g_watchdogTimeoutMs = 0;
#endif

// number of connected BLE clients
uint16_t numConnected = 0;


// Periodic liveness line. Confirms the sketch is still running, and after a
// watchdog reset the timestamp restarts near zero.
static unsigned long g_lastHeartbeat = 0;
static const unsigned long kHeartbeatMs = 30000;


static void printRangingSource(uint32_t sessionHandle, const uint8_t peerAddr[8])
{
  Serial.print("session=0x");
  Serial.print(sessionHandle, HEX);
  Serial.print(" uwbPeer=0x");
  if (peerAddr[0] < 0x10) Serial.print('0');
  Serial.print(peerAddr[0], HEX);
  if (peerAddr[1] < 0x10) Serial.print('0');
  Serial.print(peerAddr[1], HEX);

  NearbySession *sess = UWBNearbySessionManager.findBySessionHandle(sessionHandle);
  if (sess != nullptr) {
    Serial.print(" ble=");
    Serial.print(sess->bleAddress());
  }
  Serial.print(" | ");
}

/**
 * @brief notification handler for ranging data
 *
 * @param rangingData the received data
 */
void rangingHandler(UWBRangingData &rangingData) {
  Serial.print("GOT RANGING DATA - Type: "  );
  Serial.println(rangingData.measureType());

  //nearby interaction is based on Double-sided Two-way Ranging method
  if(rangingData.measureType()==(uint8_t)uwb::MeasurementType::TWO_WAY)
  {

    //get the TWR (Two-Way Ranging) measurements
    RangingMeasures twr=rangingData.twoWayRangingMeasure();
    //loop for the number of available measurements
    for(int j=0;j<rangingData.available();j++)
    {
      //if the measure is valid
      if(twr[j].status==0 && twr[j].distance!=0xFFFF)
      {
        printRangingSource(rangingData.sessionHandle(), twr[j].peer_addr);
        //print the measure
        Serial.print("Distance: ");
        Serial.println(twr[j].distance);
      }
    }

  }
  
}

/**
 * @brief callback invoked when a BLE client connects
 * 
 * @param dev , the client BLE device
 */
void clientConnected(BLEDevice dev) {
  //init the UWB stack upon first connection
  if (numConnected == 0) {
    UWB.begin(Serial, uwb::LogLevel::UWB_INFO_LEVEL);  //start the UWB engine, use Serial stream interface for logging
  }
  //increase the number of connected clients
  numConnected++;
  Serial.print("[BLE] Connected ");
  Serial.print(dev.address());
  Serial.print(" clients=");
  Serial.println(numConnected);
}

/**
 * @brief callback for BLE client disconnection
 * 
 * @param dev 
 */
void clientDisconnected(BLEDevice dev) {
  //guard against underflow: a disconnect without a matching connect would wrap
  //numConnected to 65535 and make the logs unreadable
  if (numConnected > 0) {
    numConnected--;
  }
  Serial.print("[BLE] Disconnected ");
  Serial.print(dev.address());
  Serial.print(" clients=");
  Serial.println(numConnected);
  // NOTE: do NOT call UWB.end() here. Per-session UWB cleanup is done in
  // NearbySessionManager on disconnect. The library keeps the HAL alive
  // (UWB.setKeepAlive) so the next connection does not require a full re-init.
}

/**
 * @brief callback for when a UWB session with a client is started
 * 
 * @param dev 
 */
void sessionStarted(BLEDevice dev)
{
  NearbySession &sess = UWBNearbySessionManager.find(dev);


  Serial.print("[UWB] Session started ble=");
  Serial.print(dev.address());
  if (sess.sessionID() != 0) {
    Serial.print(" session=0x");
    Serial.print(sess.sessionID(), HEX);
    // This is the LOCAL Stella MAC, not the phone's. The phone's MAC is printed
    // per-measurement from the ranging notification (twr[j].peer_addr).
    Serial.print(" localMac=0x");
    if (sess.macAddress().get(0) < 0x10) Serial.print('0');
    Serial.print(sess.macAddress().get(0), HEX);
    if (sess.macAddress().get(1) < 0x10) Serial.print('0');
    Serial.println(sess.macAddress().get(1), HEX);
  } else {
    Serial.println();
  }
}

/**
 * @brief callback for when a UWB session with a client is terminated
 * 
 * @param dev 
 */
void sessionStopped(BLEDevice dev)
{
  Serial.print("[UWB] Session stopped ble=");
  Serial.println(dev.address());
}

void setup() {
 
  Serial.begin(115200);
 

#if defined(ARDUINO_PORTENTA_C33)
  /* Only the Portenta C33 has an RGB LED. */
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, LOW);
#endif

  Serial.println("nearby interaction app start...");

  //register the callback for ranging data
  UWB.registerRangingCallback(rangingHandler);
  
  //register the callback for client connection/disconnection events
  UWBNearbySessionManager.onConnect(clientConnected);
  UWBNearbySessionManager.onDisconnect(clientDisconnected);

  //register the callbacks for client session start and stop events
  UWBNearbySessionManager.onSessionStart(sessionStarted);
  UWBNearbySessionManager.onSessionStop(sessionStopped);

#if STELLA_WATCHDOG
  //Arm the watchdog before begin(), so that call is covered too.
  //10 s leaves headroom over the longest normal blocking operation. The value is
  //clamped to the platform maximum, which is reported below.
  {
    mbed::Watchdog &wd = mbed::Watchdog::get_instance();
    const uint32_t maxTimeout = wd.get_max_timeout();
    g_watchdogTimeoutMs = 10000;
    if (g_watchdogTimeoutMs > maxTimeout) {
      g_watchdogTimeoutMs = maxTimeout;   //platform max is lower than requested
    }
    Serial.print("[WDT] max_timeout=");
    Serial.print(maxTimeout);
    Serial.print(" ms, arming at ");
    Serial.print(g_watchdogTimeoutMs);
    Serial.println(" ms");
    if (!wd.start(g_watchdogTimeoutMs)) {
      Serial.println("[WDT] ERROR: watchdog failed to start - board will NOT self-recover");
    }
  }
#endif

  //init the BLE services and characteristic, advertise with TS_DCU040 as the device name
  UWBNearbySessionManager.begin("TS_DCU040");
}

void loop() {
#if STELLA_WATCHDOG
  //Kick once per iteration.
  mbed::Watchdog::get_instance().kick();
#endif


  //periodic liveness line
  if (millis() - g_lastHeartbeat >= kHeartbeatMs) {
    g_lastHeartbeat = millis();
    Serial.print("[FIELD] alive t=");
    Serial.print(millis());
    Serial.print(" clients=");
    Serial.println(numConnected);
  }


  delay(100);

  //poll the BLE stack
  UWBNearbySessionManager.poll();
}
