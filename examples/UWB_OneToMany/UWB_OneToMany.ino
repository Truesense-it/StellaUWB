#include "StellaUWB.h"

/**
 * Two-Way Ranging - one Controller to many Controlees (multicast)
 *
 * Sets up the Arduino Stella as a multicast Controller that ranges with up to
 * four Controlees in a single session, and prints one distance per responding
 * peer.
 *
 * Hardware: 2 to 5 boards.
 *   This board : UWB_OneToMany, own MAC 0x1111
 *   Each peer  : UWB_MulticastResponder, MAC 0x2222, 0x3333, 0x4444 or 0x5555
 *
 * Peers are addressed by the destination list below. Each responder needs a
 * unique devAddr matching one entry in that list; the stock
 * UWB_MulticastResponder sketch uses 0x2222, so edit it for additional boards.
 * Fewer peers than listed is fine - absent addresses simply do not report.
 */

// Handler for ranging notifications 
void rangingHandler(UWBRangingData &rangingData) {
	Serial.print("GOT RANGING DATA - Type: ");
	Serial.println(rangingData.measureType());

	if (rangingData.measureType() == (uint8_t)uwb::MeasurementType::TWO_WAY) {
		RangingMeasures twr = rangingData.twoWayRangingMeasure();
		for (int j = 0; j < rangingData.available(); j++) {
			if (twr[j].status == 0 && twr[j].distance != 0xFFFF) {
				Serial.print("Distance: ");
				Serial.println(twr[j].distance);
			}
		}
	}
}

void setup() {
	Serial.begin(115200);
	delay(2000);

	// Define the source (this device) MAC address using 2-byte MAC
	uint8_t devAddr[] = {0x11, 0x11};
	UWBMacAddress srcAddr(UWBMacAddress::Size::SHORT, devAddr);

	// Define multiple destination MAC addresses (controlees)
	uint8_t destination1[] = {0x22, 0x22};
	uint8_t destination2[] = {0x33, 0x33};
	uint8_t destination3[] = {0x44, 0x44};
	uint8_t destination4[] = {0x55, 0x55};

	UWBMacAddress dstAddr1(UWBMacAddress::Size::SHORT, destination1);
	UWBMacAddress dstAddr2(UWBMacAddress::Size::SHORT, destination2);
	UWBMacAddress dstAddr3(UWBMacAddress::Size::SHORT, destination3);
	UWBMacAddress dstAddr4(UWBMacAddress::Size::SHORT, destination4);

	// Create a list of destination addresses
	UWBMacAddressList dest(UWBMacAddress::Size::SHORT);
	dest.add(dstAddr1);
	dest.add(dstAddr2);
	dest.add(dstAddr3);
	dest.add(dstAddr4);

	// Register the ranging notification handler before starting
	UWB.registerRangingCallback(rangingHandler);

	UWB.begin();
	Serial.println("Starting UWB ...");

	// Wait until the stack is initialised
	while (UWB.state() != 0) {
		delay(10);
	}

	Serial.println("Starting multicast session ...");

	// Set up a multicast session with ID 0x11223344
	UWBRangingOneToMany myController(0x11223344, srcAddr, dest);

	// Add the session to the session manager, in case multiple connections are needed
	UWBSessionManager.addSession(myController);

	// Prepare the session applying the default parameters
	myController.init();

	// Start the session
	myController.start();
}

void loop() {
	delay(1000);
}