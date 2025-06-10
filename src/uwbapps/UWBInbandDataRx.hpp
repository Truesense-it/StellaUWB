// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBINBANDDATARX_HPP
#define UWBINBANDDATARX_HPP

extern "C" {
    extern void *dataRcvNtfSemTx;
    extern uint8_t dataReceived[];
};


#include "UWB.hpp"
#include "UWBSession.hpp"
#include "UWBMacAddress.hpp"

/*
* This class will setup a ranging session with in-band data reception capabilities.
* When the data is received it will be signaled in the userCallback by receiving a
* notification of type DataReceived.
*/

class UWBInBandDataRx : public UWBSession {
public:   
    UWBInBandDataRx(uint32_t session_ID, UWBMacAddress srcAddr, 
                    UWBMacAddress dstAddr, uint32_t dataBlocks = 12)
        : destination(dstAddr) 
    {
        sessionID(session_ID);
        sessionType(uwb::SessionType::RANGING_WITH_DATA_TRANSFER);
        
        // Set ranging parameters using HAL types
        rangingParams.setDeviceRole(uwb::DeviceRole::RESPONDER);
        rangingParams.setDeviceType(uwb::DeviceType::CONTROLEE);
        rangingParams.setMultiNodeMode(uwb::MultiNodeMode::UNICAST);
        rangingParams.setRangingRoundUsage(uwb::RangingRoundUsage::DS_TWR);
        rangingParams.setScheduleMode(uwb::ScheduleMode::TIME_SCHEDULED);
        rangingParams.setDeviceMacAddr(srcAddr);

        // Set app parameters
        appParams.addOrUpdateParam(AppConfigId::NO_OF_CONTROLEES, 
                                    AppParamType::U32, 1);
        appParams.setDstMacAddr(dstAddr);
        setDefaults();

        // Link layer and frame config
        appParams.addOrUpdateParam(AppConfigId::LINK_LAYER_MODE, 
                                    AppParamType::U32, 
                                    static_cast<uint32_t>(uwb::LinkLayerMode::BYPASS));
        appParams.addOrUpdateParam(AppConfigId::RFRAME_CONFIG, 
                                    AppParamType::U32,
                                    static_cast<uint32_t>(uwb::RFrameConfig::SP1));

        // Vendor specific data transfer parameters
        vendorParams.addOrUpdateParam(VendorAppConfigId::SESSION_INBAND_DATA_TX_BLOCKS, 
                                       AppParamType::U32, 0);
        vendorParams.addOrUpdateParam(VendorAppConfigId::SESSION_INBAND_DATA_RX_BLOCKS, 
                                       AppParamType::U32, dataBlocks);
    }

private:
    UWBMacAddress destination;
};

#endif /* UWBINBANDDATARX */
