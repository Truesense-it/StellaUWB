// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBAppParamList.hpp"

uwb::AppConfig buildScalar(uwb::AppConfigId id, uint32_t val)
{
     uwb::AppConfig tmp;
     tmp.param_id = id;
     tmp.param_type = uwb::AppParamType::U32;
     tmp.param_value.vu32 = val;
     return tmp;
}

uwb::AppConfig buildArray(uwb::AppConfigId id, uint8_t *val, uint8_t length)
{
     uwb::AppConfig tmp;
     tmp.param_id = id;
     tmp.param_type = uwb::AppParamType::ARRAY_U8;

     uwb::AppParamValue tmpVal;
     tmpVal.au8.param_value=val;
     tmpVal.au8.param_len=length;
     tmp.param_value = tmpVal;
     return tmp;
}

// VendorAppConfig buildArray(VendorAppConfigId id, uint8_t *val, uint8_t length)
// {
//      VendorAppConfig tmp;
//      tmp.param_id = id;
//      tmp.param_type = AppParamType::ARRAY_U8;

//      AppParamValue tmpVal;
//      tmpVal.array.data = val;
//      tmpVal.array.length = length;
//      tmp.param_value = tmpVal;
//      return tmp;
// }

bool UWBAppParamList::channel(uint32_t channelNo)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::Channel, channelNo));
}

bool UWBAppParamList::preambleCodeIndex(uint32_t pci)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::PreambleCodeIndex, pci));
}

bool UWBAppParamList::sfdId(uint32_t sfdId)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::SfdId, sfdId));
}

bool UWBAppParamList::rangingDuration(uint32_t duration)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::RangingDuration, duration));
}

bool UWBAppParamList::slotPerRR(uint32_t slots)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::SlotsPerRound, slots));
}

bool UWBAppParamList::slotDuration(uint32_t duration)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::SlotDuration, duration));
}

bool UWBAppParamList::stsConfig(uint32_t sts)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::StsConfig, sts));
}

bool UWBAppParamList::stsSegments(uint8_t segments)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::NumStsSegments, segments));
}

bool UWBAppParamList::frameConfig(uint8_t config)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::RFrameConfig, config));
}

bool UWBAppParamList::rangingRoundUsage(uint8_t rru)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::RangingRoundControl, rru));
}

bool UWBAppParamList::maxRetries(uint16_t retries)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::MaxRrRetry, retries));
}

bool UWBAppParamList::uplinkTdoaTimestamp(uint8_t mode)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::UlTdoaTxTimestamp, mode));
}

bool UWBAppParamList::tdoaDeviceId(uint8_t value[], uint8_t length)
{
     return addOrUpdateParam(buildArray(uwb::AppConfigId::UlTdoaDeviceId, value, length));
}

bool UWBAppParamList::tdoaTxInterval(uint32_t interval)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::UlTdoaTxInterval, interval));
}

bool UWBAppParamList::macFcsType(uint8_t type)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::MacFcsType, type));
}

bool UWBAppParamList::noOfControlees(uint8_t number)
{
    return addOrUpdateParam(buildScalar(uwb::AppConfigId::NumControlees, number));
}
    
bool UWBAppParamList::destinationMacAddr(UWBMacAddress &addr)
{
    return addOrUpdateParam(buildArray(uwb::AppConfigId::PeerAddress, addr.getData(), addr.getSize()));
}

bool UWBAppParamList::powerId(uint8_t id)
{
     return addOrUpdateParam(buildScalar(uwb::AppConfigId::TxPowerId, id));
}

