#include <ArduinoBLE.h>
#include <StellaUWB.h>

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
 * Examples of UWB-enabled apps working with this demo:
 * 
 * NXP Trimensions AR (https://apps.apple.com/us/app/nxp-trimensions-ar/id1606143205) 
 * Qorvo Nearby Interaction (https://apps.apple.com/us/app/qorvo-nearby-interaction/id1615369084)
 * NXP android demo (source code https://github.com/nxp-uwb/UWBJetpackExample)
 * Truesense Android demo (source code https://github.com/Truesense-it/TSUwbDemo-Android)
 * 
 */

// number of connected BLE clients
uint16_t numConnected = 0;

/**
 * @brief notification handler for ranging daata
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
  if (numConnected == 0)
    UWB.begin();  //start the UWB engine, use Serial stream interface for logging
  //increase the number of connected clients
  numConnected++;
}

/**
 * @brief callback for BLE client disconnection
 * 
 * @param dev 
 */
void clientDisconnected(BLEDevice dev) {
  numConnected--;
  //deinit the UWB stack if no clients are connected
  if(numConnected==0)
    UWB.end();
}

/**
 * @brief callback for when a UWB session with a client is started
 * 
 * @param dev 
 */
void sessionStarted(BLEDevice dev)
{
  Serial.println("Session started");
}

/**
 * @brief callback for when a UWB session with a client is terminated
 * 
 * @param dev 
 */
void sessionStopped(BLEDevice dev)
{
  Serial.println("Session stopped");
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

  //init the BLE services and characteristic, advertise with TS_DCU040 as the device name
  UWBNearbySessionManager.begin( "TS_DCU040");
  

}

void loop() {

  delay(100);
  
  //poll the BLE stack
  UWBNearbySessionManager.poll();
}
