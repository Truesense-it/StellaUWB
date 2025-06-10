#ifndef UWBDLTDOATAG_HPP
#define UWBDLTDOATAG_HPP

#include "hal/uwb_hal.hpp"
#include "UWB.hpp"
#include "UWBSession.hpp"




class  UWBTdoaTag:public UWBSession {

public: 
	UWBTdoaTag(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr) 
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
		
		rangingParams.deviceRole(uwb::DeviceRole::INITIATOR );
		rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::ONE_TO_MANY);
		rangingParams.noOfControlees(1);
		rangingParams.deviceMacAddr(srcAddr);
		rangingParams.destinationMacAddr(dstAddr);
		/*mandatory as per config Digest Usage*/
		appParams.frameConfig(uwb::RfFrameConfig::Sfd_Sts);
		
		appParams.stsConfig(uwb::StsConfig::StaticSts); 
		appParams.rangingRoundUsage(uwb::RangingMethod::TDOA);
		appParams.sfdId(0);	
		
		//blink 5 times per second
		appParams.tdoaTxInterval(200);
		
		//we don't need notifications as this is a send only method
		appParams.addOrUpdateParam(UWB_SET_APP_PARAM_VALUE(uwb::AppConfigId::SessionInfoNtf, 0));
		
		appParams.preambleCodeIndex(10);

		//use less power
		appParams.powerId(30);
			
	}
		
};



#endif
