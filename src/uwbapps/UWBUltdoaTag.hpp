#ifndef UWBULTDOATAG_HPP
#define UWBULTDOATAG_HPP

//#include "uwb_internal.h"
#include "UWB.hpp"
#include "UWBSession.hpp"


class  UWBUltdoaTag:public UWBSession {

public:   
	UWBUltdoaTag(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr, uint8_t uniqueId[8], uint32_t txInterval=2000)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
		appConfig(uwb::AppConfigId::UlTdoaTxInterval, txInterval);
			
		rangingParams.deviceRole(uwb::DeviceRole::UT_TAG);
		rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::MULTICAST);
		rangingParams.noOfControlees(1);
		//rangingParams.deviceMacAddr(srcAddr);
		rangingParams.destinationMacAddr(dstAddr);
		rangingParams.deviceMacAddr(srcAddr);

		 //RFRAME type SP1
		appParams.frameConfig(uwb::RfFrameConfig::Sfd_Sts);

		//use static STS
		appParams.stsConfig(uwb::StsConfig::StaticSts);

		//TDoA Ranging Round type
		appParams.rangingRoundUsage((uint8_t)uwb::RangingMethod::TDOA);

		//set the blink repetition interval
		appParams.tdoaTxInterval(txInterval);

		appParams.addOrUpdateParam(buildScalar((uwb::AppConfigId)1, 0));//LOW_POWER_MODE - Disable

		appParams.uplinkTdoaTimestamp(2);
		

		//disable ranging data notification, TDoA tag does not report measures
		appParams.addOrUpdateParam(buildScalar(uwb::AppConfigId::SessionInfoNtf , 0));
		
		//create the unique id
		tdoaDeviceId[0] = 0x03; // Device ID
		memcpy(&tdoaDeviceId[1], uniqueId, 8); // Unique ID
		appParams.tdoaDeviceId(tdoaDeviceId, 9);

		//start frame delimiter type 0
		appParams.sfdId(0);

		//preamble code index
		appParams.preambleCodeIndex(10); // Preamble Code Index, valid values: 9, 10, 11, 12

		//16 bits CRC
		appParams.macFcsType(0);

		

		
		
		
		
		
	}
	uint8_t tdoaDeviceId[9];      		
};

#endif
