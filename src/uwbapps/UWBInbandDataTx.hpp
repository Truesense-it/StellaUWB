// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBINBANDDATATX_HPP
#define UWBINBANDDATATX_HPP

extern "C" uint8_t dataToSend[];


#include "UWB.hpp"
#include "UWBSession.hpp"
#include "UWBMacAddress.hpp"

class UWBInBandDataTx : public UWBSession {
public: 
    UWBInBandDataTx(uint32_t session_ID, UWBMacAddress srcAddr, 
                    UWBMacAddress dstAddr, uint32_t dataBlocks = 12)
        : destination(dstAddr), sequence_number(0)
    {
        setSessionID(session_ID);
        setSessionType(uwb::SessionType::RANGING_WITH_DATA_TRANSFER);
        
        // Set ranging parameters using HAL types
        rangingParams.deviceRole(uwb::DeviceRole::INITIATOR);
        rangingParams.deviceType(uwb::DeviceType::CONTROLLER);
        rangingParams.multiNodeMode(uwb::MultiNodeMode::UNICAST);
        rangingParams.rangingRoundUsage(uwb::RangingRoundUsage::DS_TWR);
        rangingParams.scheduleMode(uwb::ScheduleMode::TIME_SCHEDULED);
        rangingParams.deviceMacAddr(srcAddr);

        // Set app parameters
        appParams.addOrUpdateParam(AppConfigId::NO_OF_CONTROLEES, 
                                    AppParamType::U32, 1);
		appParams.dstMacAddr(dstAddr);
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
                                       AppParamType::U32, dataBlocks);
        vendorParams.addOrUpdateParam(VendorAppConfigId::SESSION_INBAND_DATA_RX_BLOCKS, 
                                       AppParamType::U32, 0);
    }

    uwb::Status sendData(uint8_t data[], uint16_t data_size) {
        uwb::DataPacket packet;
        packet.session_handle = sessionHdl;
        memcpy(packet.mac_address, destination.getData(), destination.getSize());
        packet.data = data;
        packet.data_size = data_size;
        packet.sequence_number = sequence_number;

        uwb::Status status = UWBHAL.sendData(packet);
        if (status != uwb::Status::SUCCESS) {
            UWBHAL.Log_E("Failed to send data");
        } else {
            sequence_number++;
        }
        return status;
    }

private:
    uint16_t sequence_number;
    UWBMacAddress destination;
};

#endif /* UWBINBANDDATATX */
