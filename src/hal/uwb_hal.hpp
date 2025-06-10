#pragma once
#include "uwb_types.hpp"
#include <functional>
#include "uwbapps/UWBAppParamList.hpp"
#include "uwbapps/UWBVendorParamList.hpp"
#include "uwbapps/UWBRangingParams.hpp"


namespace uwb {

class UwbHal {
public:
    virtual ~UwbHal() = default;

    // Core device management
    virtual Status initialize(SystemNotificationCallback systemNotificationCallback) = 0;
    virtual Status deinitialize() = 0;
    virtual Status reset() = 0;
    
    //virtual void HWreset() = 0;
    virtual Status shutdown() = 0;
    virtual void initSemaphores() = 0;
    virtual void deInitSemaphores() = 0;

    // Device information and capabilities 
    //virtual Status getDeviceInfo(DeviceInfo& info) = 0;
    virtual Status getDeviceCapability(DeviceCapabilities& capabilities) = 0;
    virtual Status getDeviceState(DeviceState& state) = 0;

    //Helpers for mobile phones
    virtual Status getUwbConfigData_Android(DeviceConfig& config) = 0;
    virtual Status getUwbConfigData_iOS(DeviceRole device_role, AccessoryConfigData& config) = 0;
    virtual Status configureDevice_Android(AndroidDeviceConfig& config) = 0;

    /**
     * @brief Configure UWB device for iOS accessory mode
     * 
     * @param config Configuration data including sharable data and vendor configs
     * @return Status Success/failure of operation
     */
    virtual Status configureDevice_iOS(ProfileConfig& config) = 0;

   
    // Session management
    virtual Status sessionInit(uint32_t session_id, SessionType type/*, uint32_t& handle*/) = 0;
    virtual Status sessionDeinit(uint32_t session_handle) = 0;
    virtual Status getSessionState(uint32_t session_handle, uint8_t& state) = 0;
    //virtual Status sessionQueryDataSize(uint32_t session_handle, uint16_t& size) = 0;

    // Session configuration
    virtual Status setRangingParams(uint32_t session_handle,  UWBRangingParams& params) = 0;
    virtual Status setAppConfig(uint32_t session_handle, AppConfigId param_id, uint32_t value) = 0;
    virtual Status setAppConfigMultiple(uint32_t session_handle, UWBAppParamList configs) = 0;
    //virtual Status setVendorAppConfig(uint32_t session_handle, UWBVendorParamList& configs) = 0;

    // Ranging control
    virtual Status startRanging(uint32_t session_handle) = 0;
    virtual Status stopRanging(uint32_t session_handle) = 0;
    virtual Status enableRangingNotifications(uint32_t session_handle, uint8_t enableRangingDataNtf, uint16_t proximityNear, uint16_t proximityFar) = 0;

    // HUS session control
    //virtual Status setControllerHusSession(uint32_t session_handle, const HusControllerConfig& config) = 0;
    //virtual Status setControleeHusSession(uint32_t session_handle, const HusControleeConfig& config) = 0;
    //virtual Status configureDataTransferPhase(uint32_t session_handle, const DataTransferPhaseConfig& config) = 0;

    // Multicast control
    //virtual Status updateControllerMulticastList(const MulticastControleeList& list) = 0;

    // Data transfer
    virtual Status sendData(uwb::DataPacket& packet) = 0;
    

    // Security & calibration
    virtual Status setStaticSts(uint32_t session_handle, uint16_t vendor_id, const std::vector<uint8_t>& sts_iv) = 0;
    //virtual Status getTrng(std::vector<uint8_t>& random_data, size_t len) = 0;

    // Raw command interface
    //virtual Status sendRawCommand(const std::vector<uint8_t>& command, std::vector<uint8_t>& response) = 0;

    // Callbacks
    //virtual Status registerRangingCallback(uint32_t session_handle, RangingCallback cb) = 0;
    //virtual Status registerErrorCallback(ErrorCallback cb) = 0;
    //virtual Status registerDataCallback(DataCallback cb) = 0;
    static UwbHal& getInstance();

    // log API
    virtual void setPrintCallback(PrintCallback logCB) = 0;
    virtual void setLogLevel(LogLevel logLevel) = 0;
    virtual void Log_D(const char *format, ...) = 0;
    virtual void Log_E(const char *format, ...) = 0;
    virtual void Log_I(const char *format, ...) = 0;
    virtual void Log_W(const char *format, ...) = 0;
    virtual void Log_Array_D(const char *message, const unsigned char *array, size_t array_len) = 0;
    virtual void Log_Array_E(const char *message, const unsigned char *array, size_t array_len) = 0;
    virtual void Log_Array_I(const char *message, const unsigned char *array, size_t array_len) = 0;
    virtual void Log_Array_W(const char *message, const unsigned char *array, size_t array_len) = 0;
    // Helper functions
    virtual uint16_t serializeDeviceConfigData(uint8_t* out_buffer, const DeviceConfig& config) = 0;
    virtual Status setDefaultCoreConfigs(void) = 0;
    //virtual void setDefaultVendorConfigs(UWBVendorParamList& vendorParams) = 0;

protected:
public: 
    SystemNotificationCallback userNotificationCallback;
    PrintCallback mPrintCallback;
};

} // namespace uwb
extern uwb::UwbHal& UWBHAL;