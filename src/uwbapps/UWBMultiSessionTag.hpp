// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBMULTISESSIONTAG_HPP
#define UWBMULTISESSIONTAG_HPP

#include "hal/uwb_hal.hpp"
#include "UWB.hpp"
#include "UWBSession.hpp"

/**
 * @brief Multi-Session Tag class for participating in multi-anchor systems
 * 
 * This class creates a UWB tag that participates in a multi-session ranging
 * system. The tag connects to an anchor running multiple concurrent sessions,
 * allowing the anchor to track multiple tags simultaneously.
 * 
 * Each tag must be configured with parameters matching its designated session
 * on the anchor, including unique session ID, MAC addresses, and preamble code.
 * 
 */
class UWBMultiSessionTag : public UWBSession {

public:   
    /**
     * @brief Construct a multi-session tag for a specific session
     * 
     * @param session_ID Unique identifier matching the anchor's session (e.g., 0x111111)
     * @param srcAddr MAC address of this tag
     * @param dstAddr MAC address of the target anchor for this session
     * @param preambleCode Preamble code (must match anchor's session configuration)
     */
    UWBMultiSessionTag(uint32_t session_ID, UWBMacAddress srcAddr, 
                       UWBMacAddress dstAddr, uint8_t preambleCode = 10)
    {
        sessionID(session_ID);
        sessionType(uwb::SessionType::RANGING);
            
        rangingParams.deviceRole(uwb::DeviceRole::INITIATOR);
        rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
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
        appParams.preambleCodeIndex(preambleCode); //must match anchor's session
        appParams.stsConfig(uwb::StsConfig::StaticSts);
        appParams.stsSegments(1);
        appParams.channel(9);
    }      		
};

#endif /* UWBMULTISESSIONTAG_HPP */