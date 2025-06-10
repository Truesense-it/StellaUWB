#include "StellaUWB.h"

/**
 * this demo shows how to setup the Arduino Stella as a uplink TDoA Tag
 * Uplink TDoA configuration is typically used for industrial and logistics use cases 
 * where a large number of tags shall be tracked, the bandwidth occupation rate 
 * shall be minimized in order to accomodate a high density of concurrent Tags 
 * and the power consumption shall be kept to the minimum in order to ensure long
 * battery life.
 * This is achieved by setting up the tags in a way that they just emit very short
 * messages (Blinks) to Anchors that are deployed in the field. Once a Blink has
 * been sent, the Tag UWB radio is turned off until the next Blink needs to be sent.
 * When using TDoA mode, Tags don't receive measurement reports from their 
 * ranging counterparts (Anchors) and, opposed to other configuration types such
 * as Two-Way Ranging, they cannot know their relative position from the Anchors.
 * Anchors receive the Blinks with their related timestamps and these informations
 * are then shared out-of-band (e.g. via WiFi or Etherenet) to a central server
 * that can use the Time Difference of Arrival at different anchors to perform
 * triangulation and locate the Tags. 
 *   
 * In order to make use of this demo you will need to setup an infrastructure where 
 * at least 4 Portenta C33 equipped with the UWB Anchor shield are setup
 * One of the Anchors shall be setup as a Synchronization Master, the others as 
 * slaves.
 * 
 */



void setup() {
  Serial.begin(115200);


  //setup the source and destination mac addresses, 2-bytes type
  uint8_t devAddr[]={0x22,0x22}; //in a setup with multiple tags, each one should have a different source address
  uint8_t destination[]={0x11,0x11};
  UWBMacAddress srcAddr(UWBMacAddress::Size::SHORT,devAddr);
  UWBMacAddress dstAddr(UWBMacAddress::Size::SHORT,destination);
  
  
  UWB.begin(); //init the UWB stack, using Serial stream as output for the logs
  
  Serial.println("Starting UWB ...");
  //wait until the stack is fully initialized
  while(UWB.state()!=0)
    delay(10);

  Serial.println("Starting session ...");
  //setup a TDOA Tag session with Id 0x11223344 with an default 2 seconds
  //transmission interval, source and destination addresses
  UWBUltdoaTag myTag(0x11223344,srcAddr,dstAddr);
  //you can change the transmission interval by passing a different value in the
  //constructor, e.g. 
  //
  //  UWBUltdoaTag myTag(0x11223344,srcAddr,dstAddr,1000);
  //
  //will configure the tag to send a Blink every 1 second
  //One can achieve the same result by using the following code:
  //
  //myTag.appParams.tdoaTxInterval(1000); //set the transmission interval to 1 second
  //

  //add to session manager, useful in case you want to manage multiple sessions
  UWBSessionManager.addSession(myTag);

  //init the session
  myTag.init();

  //start the session
  myTag.start();

}

void loop() {
  // put your main code here, to run repeatedly:
  //LED blink
#ifdef ARDUINO_ARDUINO_NANO33BLE  
  digitalWrite(p37, HIGH);
  delay(1000);
  digitalWrite(p37, LOW);
#endif  
  delay(1000);
}

