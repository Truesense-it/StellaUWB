// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef TEST2_HPP
#define TEST2_HPP


#include "hal/uwb_hal.hpp"
#include "UWB.hpp"
#include "UWBSession.hpp"

/**
 * @brief TAG (Responder) participates in multicast
 *
 * Helper session for a Stella TAG acting as RESPONDER/CONTROLEE in a
 * multicast ranging session started by a Controller. The TAG uses 
 * 2-byte short MAC addresses and DS-TWR in SP3 profile.
 */
class  MulticastResponder:public UWBSession {

public:   
	MulticastResponder(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddress dstAddr,uwb::DeviceRole role = uwb::DeviceRole::RESPONDER, uwb::DeviceType deviceType=uwb::DeviceType::CONTROLEE)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
			
		rangingParams.deviceRole(role);
		rangingParams.deviceType(deviceType);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::MULTICAST);
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