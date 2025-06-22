#ifndef UWBTRACKER_HPP
#define UWBTRACKER_HPP

#include "hal/uwb_hal.hpp"
#include "UWB.hpp"
#include "UWBSession.hpp"


class  UWBTracker:public UWBSession {

public:   
	UWBTracker(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr,uwb::DeviceRole role = uwb::DeviceRole::INITIATOR, uwb::DeviceType deviceType=uwb::DeviceType::CONTROLLER)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
			
		rangingParams.deviceRole(role);
		rangingParams.deviceType(deviceType);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::UNICAST);
		rangingParams.noOfControlees(1);
		rangingParams.deviceMacAddr(srcAddr);
		rangingParams.destinationMacAddr(dstAddr);
		rangingParams.rangingRoundUsage(uwb::RangingMethod::DS_TWR);


		appParams.frameConfig(uwb::RfFrameConfig::SP3);
		appParams.slotPerRR(25);
		appParams.rangingDuration(200);
		appParams.maxRetries(0);
		appParams.sfdId(2);
		appParams.preambleCodeIndex(10);
		appParams.stsConfig(uwb::StsConfig::StaticSts);
    	appParams.stsSegments(1);
	}      		
};

#endif
