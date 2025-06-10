#ifndef UWBULTDOATAG_HPP
#define UWBULTDOATAG_HPP

//#include "uwb_internal.h"
#include "UWB.hpp"
#include "UWBSession.hpp"


class  UWBUltdoaTag:public UWBSession {

public:   
	UWBUltdoaTag(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr, uint32_t txInterval=2000)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
			
		rangingParams.deviceRole(uwb::DeviceRole::UT_TAG);
		rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::ONE_TO_MANY);
		rangingParams.noOfControlees(1);
		rangingParams.deviceMacAddr(srcAddr);
		rangingParams.destinationMacAddr(dstAddr);

		 //RFRAME type SP1
		appParams.frameConfig(uwb::RfFrameConfig::Sfd_Sts);

		//use static STS
		appParams.stsConfig(uwb::StsConfig::StaticSts);

		//TDoA Ranging Round type
		appParams.rangingRoundUsage((uint8_t)uwb::RangingMethod::TDOA);

		
		//set the blink repetition interval
		appParams.tdoaTxInterval(txInterval);

		//disable ranging data notification, TDoA tag does not report measures
		appParams.addOrUpdateParam(buildScalar(uwb::AppConfigId::SessionInfoNtf , 0));
		
		//work on channel 9
		appParams.channel(9);

		//use preamble type 10
		appParams.preambleCodeIndex(10);

		//reduce the power output to save battery
		appParams.powerId(30);

		//start frame delimiter type 0
		appParams.sfdId(0);

		//16 bits CRC
		appParams.macFcsType(0);

		/* default UL_TDOA_TX_TIMESTAMP - 0 for LOW_POWER_MODE - Enable
		 * UL_TDOA_TX_TIMESTAMP can be enable only in LOW_POWER_MODE - Disable
		 */
		appParams.uplinkTdoaTimestamp(0);
		
		
	}      		
};

#endif
