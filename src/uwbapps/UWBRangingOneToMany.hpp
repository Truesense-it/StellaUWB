// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBRANGINGONETOMANY
#define UWBRANGINGONETOMANY


#include "UWBSession.hpp"
#include "UWBMacAddressList.hpp"

/**
 * @brief One-to-Many ranging Controller helper
 *
 * This class configures a ranging session where a single controller (initiator)
 * performs Two-Way Ranging with multiple controlees in a time-scheduled, secure
 * SP3 profile. It mirrors the style and defaults of `UWBRangingController` but
 * targets multicast.
 */
class  UWBRangingOneToMany:public UWBSession {

public:
	// Multicast constructor: accept a list of destination MAC addresses
	UWBRangingOneToMany(uint32_t session_ID, UWBMacAddress srcAddr, UWBMacAddressList dstAddr)
	{
		sessionID(session_ID);
		sessionType(uwb::SessionType::RANGING);
		rangingParams.deviceRole(uwb::DeviceRole::INITIATOR);
		rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
		rangingParams.multiNodeMode(uwb::MultiNodeMode::MULTICAST);
		rangingParams.rangingRoundUsage(uwb::RangingMethod::DS_TWR);
		rangingParams.scheduledMode(uwb::ScheduledMode::TIME_SCHEDULED);
		rangingParams.macAddrMode((uint8_t)uwb::MacAddressMode::SHORT);
		rangingParams.deviceMacAddr(srcAddr);
		rangingParams.noOfControlees(dstAddr.size());
		rangingParams.destinationMacAddr(dstAddr);
		appParams.noOfControlees(dstAddr.size());
		appParams.destinationMacAddr(dstAddr);
		appParams.frameConfig(uwb::RfFrameConfig::SP3);
		appParams.slotPerRR(25);
		appParams.rangingDuration(200);
		appParams.stsConfig(uwb::StsConfig::StaticSts);
		appParams.sfdId(2);
		appParams.preambleCodeIndex(10);
	}
};

#endif/* UWBRANGINGONETOMANY */

