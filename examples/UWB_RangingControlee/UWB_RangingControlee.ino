#include "StellaUWB.h"


/**
 * Two-Way Ranging - Responder / Controlee
 *
 * Sets up the Arduino Stella as a Two-Way Ranging Responder/Controlee and
 * prints the measured distance to its peer.
 *
 * Hardware: 2 boards.
 *   This board  : UWB_Controlee (own MAC 0x2222, peer 0x1111)
 *   Second board: UWB_Tracker   (own MAC 0x1111, peer 0x2222)
 *
 * The two sketches are already configured to address each other. For additional
 * pairs, give each board a unique devAddr and set destination to match.
 */


//define the source (this device) and destination MAC addresses, using 2-bytes MACs
uint8_t devAddr[]={0x22,0x22};
uint8_t destination[]={0x11,0x11};
UWBMacAddress srcAddr(UWBMacAddress::Size::SHORT,devAddr);
UWBMacAddress dstAddr(UWBMacAddress::Size::SHORT,destination);

//setup a session with ID 0x11223344, in this case it defines a Two-Way 
//Ranging Responder/Controlee
UWBRangingControlee myControlee(0x11223344,srcAddr,dstAddr);

// handler for ranging notifications
void rangingHandler(UWBRangingData &rangingData) {
  Serial.print("GOT RANGING DATA - Type: "  );
  Serial.println(rangingData.measureType());
  if(rangingData.measureType()==(uint8_t)uwb::MeasurementType::TWO_WAY)
  {
    
    RangingMeasures twr=rangingData.twoWayRangingMeasure();
    for(int j=0;j<rangingData.available();j++)
    {

      if(twr[j].status==0 && twr[j].distance!=0xFFFF)
      {
        Serial.print("Distance: ");
        Serial.println(twr[j].distance);  
      }
    }
   
  }
  
}

void setup() {
  Serial.begin(115200);
  
  // Wait for the serial port to open (up to 3 seconds)
  // This ensures you don't miss the initial "Starting UWB..." prints
  // and prevents the IDE from hanging on "Upload done" if the board 
  // starts running immediately before the COM port re-enumerates.
  uint32_t t = millis();
  while (!Serial && (millis() - t < 3000)) { delay(10); }
  delay(1000);
 
  // register the ranging notification handler before starting
  UWB.registerRangingCallback(rangingHandler);
  
  UWB.begin(); //start the UWB stack, use Serial for the log output
  Serial.println("Starting UWB ...");

  //wait until the stack is initialised
  while(UWB.state()!=0)
    delay(10);

  //setup the session
  Serial.println("Starting session ...");
  
  //add the session to the session manager, in case you want to manage multiple connections
  UWBSessionManager.addSession(myControlee);

  //prepare the session applying the default parameters
  myControlee.init();
  
  //start the session
  myControlee.start();
}

void loop() {

  delay(1000);
}
