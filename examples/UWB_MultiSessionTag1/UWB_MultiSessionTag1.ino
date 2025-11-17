#include "StellaUWB.h"

/**
 * Multi-Session Tag 1 Demo
 * 
 * This demo shows how to setup the Arduino Stella as Tag 1 that participates
 * in a multi-session ranging system. This tag connects to the anchor's 
 * Session 1 (0x111111).
 * 
 * Configuration must match the anchor's Session 1:
 * - Session ID: 0x111111
 * - Tag MAC: 0x2222
 * - Anchor MAC: 0x1111
 * - Preamble Code: 10
 * 
 * The anchor can simultaneously track this tag along with other tags on 
 * different sessions.
 * 
 * It expects a counterpart Portenta C33 setup as a Multi-Session Anchor
 * 
 */

// handler for ranging notifications
void rangingHandler(UWBRangingData &rangingData) {
  Serial.print("GOT RANGING DATA - Type: ");
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
  
  //define the source (this device) and destination MAC addresses, using 2-bytes MACs
  uint8_t devAddr[]={0x22,0x22};
  uint8_t destination[]={0x11,0x11};
  UWBMacAddress srcAddr(UWBMacAddress::Size::SHORT,devAddr);
  UWBMacAddress dstAddr(UWBMacAddress::Size::SHORT,destination);
  
  // register the ranging notification handler before starting
  UWB.registerRangingCallback(rangingHandler);
  
  UWB.begin(); //start the UWB stack, use Serial for the log output
  Serial.println("Starting UWB ...");

  //wait until the stack is initialised
  while(UWB.state()!=0)
    delay(10);

  Serial.println("Starting session ...");
  //setup a session with ID 0x111111, using preamble code 10
  UWBMultiSessionTag myTag(0x111111, srcAddr, dstAddr, 10);

  //add the session to the session manager
  UWBSessionManager.addSession(myTag);

  //prepare the session applying the configured parameters
  myTag.init();
  
  //start the session
  myTag.start();
}

void loop() {
  delay(1000);
}