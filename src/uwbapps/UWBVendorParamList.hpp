// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBVENDORPARAMLIST_HPP
#define UWBVENDORPARAMLIST_HPP

#include "UWBAppParamsList.hpp"
#include "hal/uwb_types.hpp"
#include "hal/uwb_hal.hpp"

class UWBVendorParamList: public UWBAppParamsList<uwb::VendorAppConfig, 
                                                 uwb::VendorAppConfigId,
                                                 uwb::AppParamType,
                                                 uwb::AppParamValue> 
{
public:
    // Constructor
    UWBVendorParamList() : UWBAppParamsList<uwb::VendorAppConfig, 
                                                 uwb::VendorAppConfigId,
                                                 uwb::AppParamType,
                                                 uwb::AppParamValue>() 
    
    {
        
        uint8_t antennaeConfigurationRx[] = { 1, 0x01, (1)};
        const uint8_t antennaeConfigurationRx_size = 3;
        uwb::VendorAppConfig antennaParam;
        antennaParam.param_id = uwb::VendorAppConfigId::ANTENNAE_CONFIGURATION_RX;
        antennaParam.param_type = uwb::AppParamType::ARRAY_U8;
        antennaParam.param_value.au8.param_value = &antennaeConfigurationRx[0];
        antennaParam.param_value.au8.param_len = antennaeConfigurationRx_size;
        addOrUpdateParam(antennaParam);
        //UWBHAL.setDefaultVendorConfigs(this);
    }
    
};

#endif /* UWBVENDORPARAMLIST */
