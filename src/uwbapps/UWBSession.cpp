// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBSession.hpp"


UWBSession::UWBSession()
{
    sessID = 0;
    //sessID = 0;
    type = uwb::SessionType::RANGING;
    isActive = false;
}


void UWBSession::sessionID(uint32_t id)
{
    sessID = id;
}



void UWBSession::sessionType(uwb::SessionType sessionType)
{
    type = sessionType;
}

uint32_t UWBSession::sessionID()
{
    return sessID;
}

// uint32_t UWBSession::sessionHandle()
// {
//     return sessID;
// }

uwb::SessionType UWBSession::sessionType()
{
    return type;
}

uwb::Status UWBSession::init()
{
    uwb::Status res = uwb::Status::SUCCESS;
    res= UWBHAL.sessionInit(sessID, type/*, sessID*/);
    if (res != uwb::Status::SUCCESS)
    {
        UWBHAL.Log_E("could not init session");
        return res;
    }
    if (appParams.getSize())
    {
        res=UWBHAL.setAppConfigMultiple(sessID, appParams);
        if (res != uwb::Status::SUCCESS)
        {
            UWBHAL.Log_E("could not set app params: %d", res);
            return res;
        }
    }
    else
        UWBHAL.Log_E("no app params");
    res=UWBHAL.setRangingParams(sessID, rangingParams);
    if (res != uwb::Status::SUCCESS)
    {
        UWBHAL.Log_E("could not set ranging params");
        return res;
    }

    // if(vendorParams.getSize())
    // {
    //     res=UWBHAL.setVendorAppConfig(sessID, vendorParams);
    //     if(res != uwb::Status::SUCCESS)
    //     {
    //         UWBHAL.Log_E("could not set vendor params -%d", res);
    //         return res;
    //     }
    // }
    // else
    //     UWBHAL.Log_E("no vendor params");

    return res;
}

uwb::Status UWBSession::deInit()
{
    return UWBHAL.sessionDeinit(sessID);
}

uwb::Status UWBSession::sendData(uwb::DataPacket& pSendData)
{
    return UWBHAL.sendData(pSendData);
}

uwb::Status UWBSession::appConfig(uwb::AppConfigId param_id, uint32_t param_value)
{
    return UWBHAL.setAppConfig(sessID, param_id, param_value);
}

uwb::Status UWBSession::enableRangingDataNtf(uint8_t enableRangingDataNtf, uint16_t proximityNear, uint16_t proximityFar)
{
    return UWBHAL.enableRangingNotifications(sessID, enableRangingDataNtf, proximityNear, proximityFar);
}

uwb::Status UWBSession::staticSts(uint16_t vendorId, uint8_t *staticStsIv)
{
    // Convert the 6-byte array to vector
    std::vector<uint8_t> stsIvVector(staticStsIv, staticStsIv + 6);
    
    return UWBHAL.setStaticSts(sessID, vendorId, stsIvVector);
}

uwb::Status UWBSession::state(uint8_t& state)
{
    return UWBHAL.getSessionState(sessID, state);
}

uwb::Status UWBSession::sendData(uint8_t *sendData, uint8_t len, uint8_t seq_num, uint8_t *dst_mac)
{
    uwb::DataPacket data;
    uint8_t offset = 0;
    
    data.session_handle = this->sessID;
    data.data_size = len;
    data.data = sendData;
    data.sequence_number = seq_num;
    for (offset = 0; offset < 8; offset++)
        data.mac_address[offset] = dst_mac[offset];
    return UWBHAL.sendData(data);
}

uwb::Status UWBSession::start()
{
    return UWBHAL.startRanging(sessID);
}
uwb::Status UWBSession::stop()
{
    return UWBHAL.stopRanging(sessID);
}

void UWBSession::applyDefaults()
{
    appParams.sfdId(2);
    appParams.slotPerRR(25);
    appParams.rangingDuration(25*8);
    appParams.stsConfig(uwb::StsConfig::StaticSts);
    appParams.stsSegments(1);
    appParams.frameConfig(3);    
}
