// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBRANGINGCONTROLLER
#define UWBRANGINGCONTROLLER


#include "UWB.hpp"
#include "UWBSession.hpp"

class  UWBRangingController:public UWBSession {

public:     
	UWBRangingController(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
		rangingParams.deviceRole(uwb::DeviceRole::INITIATOR);
		rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::UNICAST);
		rangingParams.rangingRoundUsage(uwb::RangingMethod::DS_TWR);
		rangingParams.scheduledMode(uwb::ScheduledMode::TIME_SCHEDULED);
		rangingParams.deviceMacAddr(srcAddr);
	
			
		appParams.noOfControlees(1);
	    appParams.destinationMacAddr(dstAddr);	        
		appParams.frameConfig(uwb::RfFrameConfig::SP3);
		appParams.slotPerRR(25);		
		appParams.rangingDuration(200);
		appParams.stsConfig(uwb::StsConfig::StaticSts);
		appParams.sfdId(2);
		appParams.preambleCodeIndex(10);	
		
		
	}	
};

#endif/* UWBRANGINGCONTROLLER */
