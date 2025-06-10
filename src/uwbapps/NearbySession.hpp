// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef NEARBYSESSION_HPP
#define NEARBYSESSION_HPP
#include "UWBSession.hpp"
#include "hal/uwb_types.hpp"

/* Define for App developer */
/* Specification number must be filled as mention in the developer specification */
#define SPEC_VERSION_MAJOR \
  { \
    0x01, 0x00 \
  }  // Spec major 00.01
#define SPEC_VERSION_MINOR \
  { \
    0x00, 0x00 \
  }  // Spec minor 00.00

#define SHAREABLE_DATA_LENGTH_OFFSET 5
#define SHAREABLE_DATA_HEADER_LENGTH 5

#define SHAREABLE_DATA_HEADER_LENGTH_ANDROID 14

// Enumerations for session state and device type
enum SessionState
{
    notCreated,
    notStarted,
    Started
};

enum DeviceType
{
    deviceUnknown,
    Android,
    iOS

};

typedef enum
{
    kMsg_Initialize_iOS = 0x0A,
    kMsg_Initialize_Android = 0xA5,
    kMsg_ConfigureAndStart = 0x0B,
    kMsg_Stop = 0x0C
} MessageId_t;

typedef enum
{
    kRsp_InitializedData = 0x01,
    kRsp_UwbDidStart = 0x02,
    kRsp_UwbDidStop = 0x03,
} ResponseId_t;

/**
 * @brief this class implements the device side of the Nearby Interaction with
 *  3rd Party Devices from Apple (see https://developer.apple.com/nearby-interaction/) 
 * the implementation also works with UWB-enabled Android devices by using different
 * commnd IDs
 * 
 */

class NearbySession : public UWBSession
{

public:
    NearbySession()
    {
        devType = deviceUnknown;
        sessState = notCreated;
    }
    NearbySession(BLEDevice dev)
    {
        devType = deviceUnknown;
        sessState = notCreated;
        bleDev = dev;
    }

    void bleDevice(BLEDevice dev) { bleDev = dev; }
    BLEDevice bleDevice(void) { return bleDev; }
    String bleAddress() { return bleDev.address(); }
    void macAddress(UWBMacAddress addr) { macAddr = addr; }
    UWBMacAddress macAddress(void) { return macAddr; }
    void sessionState(SessionState state) { sessState = state; }
    SessionState sessionState(void) { return sessState; }
    void deviceType(DeviceType type)
    {
        devType = type;
    }
    DeviceType deviceType(void) { return devType; }

    uint8_t startAndroid(uint8_t* uwb_data_content)
    {
        profileInfo.mac_addr[0] = macAddress().get(0);
        profileInfo.mac_addr[1] = macAddress().get(1);
        //profileInfo.profile_id = 1;//uwb::ProfileId::Profile_1;
        //UWBHAL.Log_MAU8_I("mac addr :", profileInfo.mac_addr, 2);
        uwb::AndroidDeviceConfig andConfig;
        andConfig.config_data = uwb_data_content + 1;
        andConfig.config_data_length = ( SHAREABLE_DATA_HEADER_LENGTH_ANDROID);
        andConfig.profile_info = profileInfo;
        andConfig.vendor_configs = {};  
        andConfig.debug_configs = {};
        

        uwb_status = UWBHAL.configureDevice_Android(andConfig);
        if (uwb_status != uwb::Status::SUCCESS) {
            UWBHAL.Log_E("Phone data not configured");
            /* If the status if HPD wake up then try to do it one more time */
            if (uwb::Status::HPDWKUP == uwb_status) {
                UWBHAL.Log_W("Device woke up from HPD");
                UWBHAL.setDefaultCoreConfigs();
                uwb_status = UWBHAL.configureDevice_Android(andConfig);
                if (uwb_status != uwb::Status::SUCCESS) {
                    UWBHAL.Log_E("Shareable data not configured");
                    return uwb_status;
                }
            } else
            {
                UWBHAL.Log_E("Shareable data not configured");
                return uwb_status;
            }
        } else
        {
            UWBHAL.Log_I("Phone data configured");
            //sessionHandle(profileInfo.session_handle);
            sessionID(profileInfo.session_handle);
            sessionState(Started);
            return uwb::Status::SUCCESS;
        }
        return uwb::Status::FAILED;
    }

    uint8_t configIOS()
    {
        /* Application related definitions */
        uint8_t SpecMajorVersion[] = SPEC_VERSION_MAJOR;
        uint8_t SpecMinorVersion[] = SPEC_VERSION_MINOR;

        memcpy(UserConfigData_iOS.customer_spec_major_ver, SpecMajorVersion, sizeof(SpecMajorVersion));
        memcpy(UserConfigData_iOS.customer_spec_minor_ver, SpecMinorVersion, sizeof(SpecMinorVersion));
        UserConfigData_iOS.preferred_update_rate = uwb::PreferredUpdateRate::UserInteractive;

        deviceType(iOS);

        /* UWB related definitions */
        //uwb::AccessoryConfigData cfgIos; 
        //UserConfigData_iOS.uwb_config_data.spec_version_major
        //cfgIos.spec_version_major= SPEC_VERSION_MAJOR;
        //cfgIos.spec_version_major[0] = SPEC_VERSION_MAJOR[0];
        //cfgIos.spec_version_minor[1] = SPEC_VERSION_MAJOR[1];
        
        uwb_status = UWBHAL.getUwbConfigData_iOS(uwb::DeviceRole::INITIATOR, UserConfigData_iOS.uwb_config_data);
        if (uwb_status != uwb::Status::SUCCESS)
        {
            UWBHAL.Log_E("GetUwbConfigData configuration failed");
            /* If the status if HPD wake up then try to do it one more time */
            if (uwb::Status::HPDWKUP == uwb_status)
            {
                UWBHAL.Log_W("Device woke up from HPD");
                UWBHAL.setDefaultCoreConfigs();

                uwb_status = UWBHAL.getUwbConfigData_iOS(uwb::DeviceRole::INITIATOR, UserConfigData_iOS.uwb_config_data);                
            }
        }
        if (uwb_status != uwb::Status::SUCCESS)
        {
            UWBHAL.Log_E("GetUwbConfigData configuration failed");
        }
        /* Build BLE Message, to be build depending on the iOS application message stream
         * In example application, it contains Message ID + Accessory configuration data */
        BLEmessage_iOS[0] = kRsp_InitializedData; /* Response ID */

        /* Warning: this copy works correctly because the UserConfigData_iOS does not need
         * padding, if more fields were to be added and don't fit the memory alignment then extra
         * bytes could be added and more data than required would be sent. To fix that a serialize
         * utils function would be required or use the packed attribute
         */
        memcpy(&BLEmessage_iOS[1], (uint8_t *)&UserConfigData_iOS, sizeof(UserConfigData_iOS));
        UWBMacAddress uMac;
        uMac.set(0, UserConfigData_iOS.uwb_config_data.device_mac_addr[0]);
        uMac.set(1, UserConfigData_iOS.uwb_config_data.device_mac_addr[1]);
        macAddress(uMac);
        if (shouldUpdateAccessory())
        {
            dataLen = 38;
        }
        else
        {
            dataLen = 36;
        }
        return uwb::Status::SUCCESS;
    }

    uint8_t startIOS(uint8_t *data)
    {
#define SHAREABLE_DATA_LENGTH_OFFSET 5
#define SHAREABLE_DATA_HEADER_LENGTH 5
        uwb::ProfileInfo profInfo;
        uint8_t dataLen = *(data + SHAREABLE_DATA_LENGTH_OFFSET) + SHAREABLE_DATA_HEADER_LENGTH;
        profInfo.device_role = uwb::DeviceRole::INITIATOR;
        profInfo.device_type = uwb::DeviceType::CONTROLLER;
        profInfo.mac_addr[0] = macAddress().get(0);
        profInfo.mac_addr[1] = macAddress().get(1);
        //UWBHAL.Log_Array_D("data:", data, 30);
        //Serial.println("data len:");
        //Serial.println(dataLen);
        //UWBHAL.Log_D("incoming shareable data len: %hhu", dataLen);
        std::vector<uint8_t> shareData(data, data+dataLen);
        
        uwb::ProfileConfig profileCfg;
        profileCfg.sharable_data = shareData;

        profileCfg.profile_info.session_handle = profInfo.session_handle;
        profileCfg.profile_info.profile_id = profInfo.profile_id;
        profileCfg.profile_info.device_role = profInfo.device_role;
        profileCfg.profile_info.device_type = profInfo.device_type;
        profileCfg.profile_info.mac_addr[0] = profInfo.mac_addr[0];
        profileCfg.profile_info.mac_addr[1] = profInfo.mac_addr[1];
        UWBHAL.Log_Array_D("mac addr :", profileCfg.profile_info.mac_addr, 2);
        //profileCfg.vendor_configs = {};
        //profileCfg.debug_configs = {};
        //UWBHAL.Log_MAU8_I("mac addr :", profileInfo.mac_addr, 2);
        uwb_status=UWBHAL.configureDevice_iOS(profileCfg);
        if (uwb_status != uwb::Status::SUCCESS)
        {
            UWBHAL.Log_E("Shareable data not configured");
            /* If the status if HPD wake up then try to do it one more time */
            if (uwb::Status::HPDWKUP == uwb_status)
            {
                UWBHAL.Log_W("Device woke up from HPD");
                UWBHAL.setDefaultCoreConfigs();
                //uwb_status = UWBHAL.configIOSData(data + 1, *(data + SHAREABLE_DATA_LENGTH_OFFSET) + SHAREABLE_DATA_HEADER_LENGTH, &profileInfo, 0, NULL, 0, NULL);
                uwb_status=UWBHAL.configureDevice_iOS(profileCfg);
                
                if (uwb_status != uwb::Status::SUCCESS)
                {
                    UWBHAL.Log_E("Shareable data not configured");
                    return uwb::Status::FAILED;
                }
            }
            else
            {
                UWBHAL.Log_E("Shareable data not configured");
            }
        }
        else
        {
            UWBHAL.Log_D("Shareable data configured");
            UWBHAL.Log_D("session handle: %d", profileCfg.profile_info.session_handle);
            //sessionHandle(profileCfg.profile_info.session_handle);
            sessionID(profileCfg.profile_info.session_handle);
            sessionState(Started);
        }
        return uwb::Status::SUCCESS;
    }

    uint8_t configAndroid(void)
    {
        deviceType(Android);
        uwb::DeviceConfig cfgAndroid;
        /* UWB related definitions */
        uwb_status = UWBHAL.getUwbConfigData_Android(cfgAndroid);
        if (uwb_status != uwb::Status::SUCCESS)
        {
            UWBHAL.Log_E("GetUwbConfigData configuration failed");
            return uwb_status;
        }
    
        /* Store generated own device UWB MAC address */
        UWBMacAddress uMac;
        uMac.set(0, cfgAndroid.device_mac_addr[0]);
        uMac.set(1, cfgAndroid.device_mac_addr[1]);
        macAddress(uMac);
        dataLen = UWBHAL.serializeDeviceConfigData(&BLEmessage_Android[1],cfgAndroid);
        BLEmessage_Android[0] = kRsp_InitializedData; /* Response ID */
        dataLen = (uint16_t)(dataLen + 1);
        return uwb_status;
    }
    bool shouldUpdateAccessory(void)
    {
        return (UserConfigData_iOS.uwb_config_data.spec_version_minor[0] == 0x01) && (UserConfigData_iOS.uwb_config_data.spec_version_minor[1] == 0x00);
    }

    uint16_t configLen(void)
    {
        return dataLen;
    }

    uint8_t *config()
    {
        if (devType == iOS)
            return BLEmessage_iOS;
        else if (devType == Android)
            return BLEmessage_Android;

        return nullptr;
    }

private:
    BLEDevice bleDev;
    DeviceType devType;
    SessionState sessState;
    UWBMacAddress macAddr;
    
    struct uwb::ProfileInfo profileInfo;
    //UwbDeviceConfigData_t uwb_data_content;
    struct uwb::iOSAccessoryConfigData  UserConfigData_iOS = {0};
    //UwbDeviceConfigData_t UwbDeviceConfigData = { 0 };
    uint8_t BLEmessage_iOS[1 + 37];
    uint8_t BLEmessage_Android[1 + 18];
    uint16_t dataLen;
    uwb::Status uwb_status;
   
};
#endif
