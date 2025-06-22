#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <string.h>
#include <stddef.h>
//#include "uwb_iot_ftr.h"

namespace uwb {



// Core constants 
constexpr  uint8_t MAX_RESPONDERS = 12;
constexpr uint8_t MAX_MAC_ADDR_LEN = 8;
constexpr uint8_t MAX_TDOA_MEASURES = 16;
constexpr uint8_t MAX_OWR_AOA_MEASURES = 8;
constexpr uint8_t MAX_APP_DATA_SIZE = 116;
constexpr uint8_t MAC_SHORT_ADD_LEN = 2;
constexpr uint8_t MAC_EXT_ADD_LEN = 8;
constexpr uint32_t UWB_CMD_TIMEOUT = 2000;

// Result status codes
enum Status : uint8_t {
    SUCCESS = 0x00,
    REJECTED = 0x01,
    FAILED = 0x02,
    NOT_INITIALIZED = 0x03,
    INVALID_PARAM = 0x04,
    INVALID_RANGE = 0x05,
    SESSION_NOT_EXIST = 0x11,
    INVALID_PHASE = 0x12,
    SESSION_ACTIVE = 0x13,
    MAX_SESSIONS_EXCEEDED = 0x14,
    SESSION_NOT_CONFIGURED = 0x15,
    BUFFER_OVERFLOW = 0xFA,
    PKT_SENT = 0xFB,
    HPDWKUP = 0xFC,
    TIMEOUT = 0xFD
};

// Device roles
enum DeviceRole : uint8_t {
    RESPONDER = 0,
    INITIATOR = 1,
    UT_SYNC_ANCHOR = 2,
    UT_ANCHOR = 3,
    UT_TAG = 4,
    ADVERTISER = 5,
    OBSERVER = 6,
    DL_TDOA_ANCHOR = 7,
    DL_TDOA_TAG = 8
};

// Session types 
enum SessionType : uint8_t {
    RANGING = 0x00,
    RANGING_WITH_DATA = 0x01,
    DATA_TRANSFER = 0x02,
    RANGING_ONLY = 0x03,
    DATA_ONLY = 0x04,
    RANGING_AND_DATA = 0x05,

};

// Device types
enum DeviceType : uint8_t {
    CONTROLEE = 0,
    CONTROLLER = 1
};

// Ranging modes
enum class RangingMethod : uint8_t {
    TDOA = 0,
    SS_TWR = 1,
    DS_TWR = 2,
    SS_TWR_NO_DEFER = 3,
    DS_TWR_NO_DEFER = 4,
    DL_TDOA = 5,
    OWR_AOA = 6
};

enum class RangingMode : uint8_t {
    SS_TWR = 0,              // Single-Sided Two Way Ranging
    DS_TWR = 1,              // Double-Sided Two Way Ranging
    SS_TWR_NO_DEFER = 2,     // Single-Sided TWR without defer
    DS_TWR_NO_DEFER = 3,     // Double-Sided TWR without defer
    TDOA = 4,                // Time Difference of Arrival
    DL_TDOA = 5,             // Downlink Time Difference of Arrival
    OWR_AOA = 6              // One Way Ranging with Angle of Arrival
};

enum HusConfigMode : uint8_t {
    HUS_UNICAST = 0x00,          // Single peer communication
    HUS_MULTICAST = 0x01,        // Multiple peer communication
    HUS_ONE_TO_MANY = 0x02,      // Broadcasting configuration
    HUS_PROVISIONED = 0x03       // Pre-configured setup
};

enum HusSecurityMode : uint8_t {
    HUS_NO_SECURITY = 0x00,      // No security features enabled
    HUS_STATIC_STS = 0x01,       // Static STS security
    HUS_DYNAMIC_STS = 0x02,      // Dynamic STS security
    HUS_PROVISIONED_STS = 0x03   // Pre-provisioned STS keys
};

enum MultiNodeMode : uint8_t {
    UNICAST = 0x00,          // One-to-one communication
    MULTICAST = 0x01,        // One-to-many communication
    ONE_TO_MANY = 0x02,      // One controller to multiple controlees
    MANY_TO_MANY = 0x03      // Multiple controllers and controlees
};

enum  ScheduledMode : uint8_t {
    CONTENTION = 0x00,         // Contention-based scheduling
    TIME_SCHEDULED = 0x01,     // Time-based scheduled access
    BLOCK_SCHEDULED = 0x02,    // Block-based scheduled access
    HYBRID = 0x03             // Hybrid scheduling mode
};

enum class AppParamType : uint8_t {
    UNKNOWN = 0,    // Maps to kUWB_APPPARAMS_Type_Unknown
    U32 = 4,        // Maps to kUWB_APPPARAMS_Type_u32 
    ARRAY_U8 = 5    // Maps to kUWB_APPPARAMS_Type_au8
};

enum class DebugParamsType : uint8_t {
    UNKNOWN = 0,    // Maps to kUWB_DEBUGPARAMS_Type_Unknown
    U8 = 1,         // Maps to kUWB_DEBUGPARAMS_Type_u8
    U16 = 2,        // Maps to kUWB_DEBUGPARAMS_Type_u16
    U32 = 4         // Maps to kUWB_DEBUGPARAMS_Type_u32
};

enum class MacAddressMode : uint8_t
{
    SHORT = 0,
    LONG = 2,
} ;

enum class DeviceState : uint8_t {
    UNKNOWN = 0,               // Unknown or invalid state
    NOT_INITIALIZED = 1,       // Device not initialized (maps to UWBD_DEVICE_NOT_INITIALIZED)
    INITIALIZED = 2,          // Device initialized (maps to UWBD_DEVICE_INITIALIZED)
    ACTIVE = 3,               // Device active/running (maps to UWBD_DEVICE_ACTIVE)
    ERROR = 4,               // Device in error state (maps to UWBD_DEVICE_ERROR)
    SUSPENDED = 5            // Device suspended (maps to UWBD_DEVICE_SUSPEND)
};

// Define Logging Levels
enum class LogLevel : int { 
    UWB_SILENT_LEVEL,
    UWB_ERROR_LEVEL,
    UWB_WARN_LEVEL,   
    UWB_INFO_LEVEL,
    UWB_DEBUG_LEVEL,
    UWB_TX_LEVEL,
    UWB_RX_LEVEL
} ;


enum AppConfigId : uint8_t {
    DevType = 0x00,               // Controller/Controlee role
    RngMode = 0x01,             // Ranging protocol mode
    StsConfig = 0x02,               // STS configuration
    MultiNode = 0x03,           // Single/Multi node mode
    Channel = 0x04,                 // UWB channel number
    NumControlees = 0x05,           // Number of controlees
    LocalAddress = 0x06,            // Device MAC address
    PeerAddress = 0x07,             // Destination MAC address
    SlotDuration = 0x08,            // Duration of ranging slot
    RangingDuration = 0x09,         // Overall ranging duration
    StsIndex = 0x0A,                // STS index value
    MacFcsType = 0x0B,              // MAC FCS type
    RangingRoundControl = 0x0C,     // Ranging round control flags
    AoaResultReq = 0x0D,            // AoA result requirements
    SessionInfoNtf = 0x0E,          // Session info notification config
    ProximityNear = 0x0F,           // Near proximity threshold
    ProximityFar = 0x10,            // Far proximity threshold
    DevRole = 0x11,              // Device role in session
    RFrameConfig = 0x12,            // Ranging frame configuration  
    RssiReporting = 0x13,           // RSSI reporting control
    PreambleCodeIndex = 0x14,       // Preamble code index
    SfdId = 0x15,                   // Start Frame Delimiter ID
    PsduDataRate = 0x16,            // PSDU data rate
    PreambleDuration = 0x17,        // Preamble duration
    LinkMode = 0x18,                // Link layer mode
    DataRepetition = 0x19,          // Data repetition count
    RangingTimeStruct = 0x1A,       // Ranging time structure
    SlotsPerRound = 0x1B,           // Slots per ranging round
    AoaBoundConfig = 0x1D,          // AoA boundary configuration
    PrfMode = 0x1F,                 // Pulse Repetition Frequency mode
    CapSizeRange = 0x20,            // CAP size range
    SchedMode = 0x22,           // Scheduling mode
    KeyRotation = 0x23,             // Key rotation enable
    KeyRotationRate = 0x24,         // Key rotation rate
    SessionPriority = 0x25,         // Session priority
    MacAddressMode = 0x26,          // MAC address mode
    VendorId = 0x27,                // Vendor identifier
    StaticStsIv = 0x28,             // Static STS IV
    NumStsSegments = 0x29,          // Number of STS segments
    MaxRrRetry = 0x2A,              // Max ranging retry count
    UwbInitTime = 0x2B,             // UWB initialization time
    HoppingMode = 0x2C,             // Channel hopping mode
    BlockStride = 0x2D,             // Block striding length
    ResultReport = 0x2E,            // Result report configuration
    BprfPhrRate = 0x2F,             // BPRF PHR data rate
    MaxMeasurements = 0x30,         // Max measurements count
    UlTdoaTxInterval = 0x33,        // UL-TDoA TX interval
    UlTdoaRandomWindow = 0x34,      // UL-TDoA random window
    StsLength = 0x35,               // STS length configuration
    SuspendRanging = 0x36,          // Suspend ranging rounds
    UlTdoaDeviceId = 0x38,          // UL-TDoA device ID
    UlTdoaTxTimestamp = 0x39,       // UL-TDoA TX timestamp
    MinFramesPerRr = 0x3A,          // Min frames per ranging round
    MtuSize = 0x3B,                 // Maximum Transfer Unit size
    InterFrameInterval = 0x3C,      // Inter-frame interval
    DlTdoaMethod = 0x3D,            // DL-TDoA ranging method
    SessionKey = 0x45,              // Session key
    SubSessionKey = 0x46,           // Sub-session key
    DataTransferStatus = 0x47,      // Data transfer status config
    SessionTimeBase = 0x48,         // Session time base
    EndpointConfig = 0x4C,           // Application endpoint config
    TxPowerId = 0xF2,
};

enum DeviceConfigId : uint16_t {
    LOW_POWER_MODE = 0x01,                // 0:DISABLE, 1:ENABLE
    DPD_WAKEUP_SRC = 0xE402,             // DPD wakeup source bit1: GPIO1, bit3: GPIO3
    WTX_COUNT_CONFIG = 0xE403,           // WTX count, 20>= wtx count <=120
    DPD_ENTRY_TIMEOUT = 0xE404,          // DPD entry timeout in ms (default = 500ms)
    WIFI_COEX_FEATURE = 0xE405,          // WiFi coexistence feature configuration
    RX_GPIO_ANTENNA_SELECTION = 0xE406,   // RX Antenna selection configuration
    TX_BASE_BAND_CONFIG = 0xE426,        // 0:DISABLE, 1:ENABLE
    DDFS_TONE_CONFIG = 0xE427,           // DDFS tone configuration
    TX_PULSE_SHAPE_CONFIG = 0xE428,      // Preamble pulse shape settings
    CLK_CONFIG_CTRL = 0xE430,            // Clock source configuration
    HOST_MAX_UCI_PAYLOAD_LENGTH = 0xE431, // Maximum UCI payload length
    NXP_EXTENDED_NTF_CONFIG = 0xE433,    // NXP extended notification config
    CLOCK_PRESENT_WAITING_TIME = 0xE434,  // Max waiting time for clock present
    INITIAL_RX_ON_OFFSET_ABS = 0xE435,   // Initial RX on offset absolute
    INITIAL_RX_ON_OFFSET_REL = 0xE436,   // Initial RX on offset relative
    WIFI_COEX_UART_USER_CFG = 0xE437,    // UART WiFi coexistence config
    PDOA_CALIB_TABLE_DEFINE = 0xE446,    // PDoA calibration table definition
    ANTENNA_RX_IDX_DEFINE = 0xE460,      // RX antenna index definition
    ANTENNA_TX_IDX_DEFINE = 0xE461,      // TX antenna index definition
    ANTENNAE_RX_PAIR_DEFINE = 0xE462,    // RX antenna pair definition
    WIFI_CO_EX_CH_CFG = 0xE464           // WiFi coexistence channel config
};

enum class VendorAppConfigId : uint8_t {
    MAC_PAYLOAD_ENCRYPTION = 0x00,
    ANTENNAE_CONFIGURATION_TX = 0x02,
    ANTENNAE_CONFIGURATION_RX = 0x03,
    RAN_MULTIPLIER = 0x20,
    STS_LAST_INDEX_USED = 0x21,
    CIR_LOG_NTF = 0x30,
    PSDU_LOG_NTF = 0x31,
    RSSI_AVG_FILT_CNT = 0x40,
    CIR_CAPTURE_MODE = 0x60,
    RX_ANTENNA_POLARIZATION_OPTION = 0x61,
    SESSION_SYNC_ATTEMPTS = 0x62,
    SESSION_SCHED_ATTEMPTS = 0x63,
    SCHED_STATUS_NTF = 0x64,
    TX_POWER_DELTA_FCC = 0x65,
    TEST_KDF_FEATURE = 0x66,
    TX_POWER_TEMP_COMPENSATION = 0x67,
    WIFI_COEX_MAX_TOLERANCE_COUNT = 0x68,
    ADAPTIVE_HOPPING_THRESHOLD = 0x69,
    AUTHENTICITY_TAG = 0x6E,
    RX_NBIC_CONFIG = 0x6F,
    MAC_CFG = 0x70,
    SESSION_INBAND_DATA_TX_BLOCKS = 0x71,
    SESSION_INBAND_DATA_RX_BLOCKS = 0x72,
    ANTENNAE_SCAN_CONFIGURATION = 0x74,
    DATA_TRANSFER_TX_STATUS_CONFIG = 0x75,
    ULTDOA_MAC_FRAME_FORMAT = 0x76,
    WRAPPED_RDS = 0x79,
    RFRAME_LOG_NTF = 0x7B,
    TX_ADAPTIVE_PAYLOAD_POWER = 0x7F,
    SWAP_ANTENNA_PAIR_3D_AOA = 0x80,
    RML_PROXIMITY_CONFIG = 0x81,
    CSA_MAC_MODE = 0x82,
    CSA_ACTIVE_RR_CONFIG = 0x83,
    FOV_ENABLE = 0x84,
    AZIMUTH_FIELD_OF_VIEW = 0x85
};
enum class MeasurementType : uint8_t {
    ONE_WAY = 0x00,       // One Way Ranging
    TWO_WAY = 0x01,       // Two Way Ranging
    DL_TDOA = 0x02,       // Downlink Time Difference of Arrival
    OWR_WITH_AOA = 0x03   // One Way Ranging with Angle of Arrival
};

enum class NotificationType : uint8_t {
    RANGING_DATA = 0,                    // Ranging Data Notification
    DATA_TRANSMIT_NTF = 1,              // Data Transmit Notification
    PER_SEND = 2,                       // PER Packet Sent Notification
    PER_RCV = 3,                        // PER receive operation completed
    GENERIC_ERROR_NTF = 4,              // Generic Error Notification
    DEVICE_RESET = 5,                    // Device Reset Notification
    RFRAME_DATA = 6,                    // RFRAME Data Notification
    RECOVERY_NTF = 7,                    // Recovery Notification
    SCHEDULER_STATUS_NTF = 8,           // Scheduler Status Notification
    SESSION_DATA = 9,                   // Session Data Notification
    RF_LOOPBACK_RCV = 10,              // RF loopback test data
    MULTICAST_LIST_NTF = 11,           // Multicast list notification
    OVER_TEMP_REACHED = 12,            // Over Temperature reached
    BLINK_DATA_TX_NTF = 13,           // Blink data tx notification
    DATA_TRANSFER_PHASE_CONFIG_NTF = 14, // Data transfer phase config
    ACTION_APP_CLEANUP = 15,           // Application cleanup
    TEST_MODE_LOOP_BACK_NTF = 16,      // Loopback Status Data
    UWB_TEST_PHY_LOG_NTF = 17,        // PHY LOG NTF 
    TEST_RX_RCV = 18,                  // TEST RX notification
    DATA_RECV_NTF = 19,               // Data receive
    CIR_DATA_NTF = 20,                // CIR notification data
    DATA_LOGGER_NTF = 21,             // Data logger ntf
    PSDU_DATA_NTF = 22,               // PSDU log ntf
    RANGING_TIMESTAMP_NTF = 23,        // RANGING timestamp ntf
    DATA_RCV_NTF = 24,                // Data receive notification
    RADAR_RCV_NTF = 25,               // Radar rcv ntf
    TEST_RADAR_ISO_NTF = 26,          // Test radar Isolation ntf
    WIFI_COEX_IND_NTF = 27,           // WiFi coexistence indication
    MAX_ACTIVE_GRANT_DURATION_EXCEEDED_WAR_NTF = 28,  // Max Active Grant Duration Warning
    RANGING_CCC_DATA = 29             // Ranging CCC Data Notification
};

// Core data structures
struct MacAddress {
    std::array<uint8_t, MAX_MAC_ADDR_LEN> addr;
    bool is_short;  // true=2 bytes, false=8 bytes
};


typedef struct /*__attribute__((packed))*/ {  // TWR measurements (maps to phRangingMesr_t)
    uint8_t peer_addr[8];
    uint8_t status;
    uint8_t nlos;            
    uint8_t aoa_azimuth_fom;
    uint8_t no_of_pdoa_measures;
    uint8_t aoa_elevation_fom;
    uint8_t aoa_dest_azimuth_fom;
    uint16_t distance;
    int16_t aoa_azimuth;     
    int16_t aoa_elevation;
    int16_t aoa_dest_azimuth;
    int16_t aoa_dest_elevation; 
    uint8_t aoa_dest_elevation_fom;
    uint8_t slot_index;
    int16_t rssi_rx1;
    int16_t rssi_rx2;
    uint16_t distance_2;
    int16_t pdoaFirst;
    uint16_t pdoaFirstIndex;
    int16_t pdoaSecond;
    uint16_t pdoaSecondIndex;
       
} twr_mesr;

typedef struct /*__attribute__((packed))*/ {  // TDoA measurements (maps to phRangingMesrTdoa_t)
    uint8_t peer_addr[8];
    uint8_t status;
    uint8_t message_control;
    uint8_t frame_type;
    uint8_t nlos;
    int16_t aoa_azimuth;
    uint8_t aoa_azimuth_fom;
    int16_t aoa_elevation;
    uint8_t aoa_elevation_fom;
    uint32_t frame_number;
    uint64_t rx_timestamp;
    uint64_t ul_tdoa_device_id;
    uint64_t tx_timestamp;
} tdoa_mesr;

typedef struct /*__attribute__((packed))*/ {  // DL-TDoA measurements (maps to phRangingMesrDlTdoa_t)
    uint8_t peer_addr[8];
    uint8_t status;
    uint8_t message_type;
    uint16_t message_control;
    uint16_t block_index;
    uint8_t round_index;
    uint8_t nlos;
    int16_t aoa_azimuth;
    uint8_t aoa_azimuth_fom;
    int16_t aoa_elevation;
    uint8_t aoa_elevation_fom;
    uint8_t rssi;
    uint64_t tx_timestamp;
    uint64_t rx_timestamp;
    int16_t cfo_anchor;
    int16_t cfo;
    uint32_t reply_time_initiator;
    uint32_t reply_time_responder;
    uint16_t initiator_responder_tof;
    uint8_t anchor_location[1];
    /** Active Ranging Rounds */
    uint8_t active_ranging_rounds[0x0F];
} dltdoa_mesr;

typedef struct {  // OWR AoA measurements (maps to phRangingMesrOwrAoa_t)
    uint8_t peer_addr[8];
    uint8_t status;
    uint8_t nlos;
    uint8_t frame_seq_num;
    uint16_t block_index;
    int16_t aoa_azimuth;
    uint8_t aoa_azimuth_fom;
    int16_t aoa_elevation;
    uint8_t aoa_elevation_fom;
} owr_aoa_mesr;

typedef struct extraRxInfoMesr_t
{
    /** RX mode */
    uint8_t rxMode;
    /** Number of RX antenna to follow */
    uint8_t num_of_rx_antennaRxInfo;
    /** RX antenna pair */
    uint8_t rx_antennaIdRxInfo[8];//max number of antenna pairs
} extraRxInfoMesr;


typedef struct extraRxInfoDebugNtf_t
{
    /** RX mode */
    uint8_t rxModeDebugNtf;
    /** Number of RX antenna to follow */
    uint8_t num_of_rx_antennaDebugNtf;
    /** RX antenna pair */
    uint8_t rx_antennaIdDebugNtf[8];//max number of antenna pairs
} extraRxInfoDebugNtf;

typedef struct extraAoaPdoaMesr_t
{
    /** Angle of arrival */
    int16_t angleOfArrival;
    /** Phase difference of arrival */
    int16_t pdoa;
    /** Phase difference of arrival index in the whole CIR */
    uint16_t pdoaIndex;
    /** This parameter indicates the presence or absence
     * of the peer device in FoV for user configured
     * Horizontal Rx Antenna Pair as defined in 'AZIMUTH_FIELD_OF_VIEW'
     * 0x00: Peer device is not present in FoV
     * 0x01: Peer device is present in FoV
     * This field would be displayed only when NXP Specific Data Type is 0xA0
    */
    uint8_t aoaFovFlag;

} extraAoaPdoaMesr;

typedef struct extraSnrPathIndexMesr_t
{
    /** Signal-to-Noise (SNR) of the First Path in dB*/
    uint8_t rxSnrFirstPath;
    /** Signal-to-Noise (SNR) of the Main Path in dB */
    uint8_t rxSnrMainPath;
    /** First path index in the whole CIR */
    int16_t rx_FirstPathIndex;
    /** Main path index in the whole CIR */
    int16_t rx_MainPathIndex;
} extraSnrPathIndexMesr;

typedef struct extraTwoWayRangingMesr_t
{
    /** AoA / PDoA measurements per RX */
    extraAoaPdoaMesr aoaPdoaMesr_twr[8]; //max number of antenna pairs
    /** SNRFirst / SNRMain / FirstIndex / Main Index measurements per RX */
    extraSnrPathIndexMesr snrPathIndexMesr_twr[8]; //max number of antenna pairs
    /** Range or distance between the device and target
     *  Set to 0 for Initiator: distance-2 is reported using the RFM
     *  pair on responder only in AOA_RFM mode */
    uint16_t distance_2;
} extraTwoWayRangingMesr;

typedef struct extraTwoWayRangingData_t
{
    
    /** WiFi co-existence status*/
    uint8_t wifiCoExStatus;
    /** Rx Antenna Info for AoA Measurements*/
    extraRxInfoMesr rxInfoMesr_twr;
    /** Rx Antenna Info For Debug Notifications*/
    extraRxInfoDebugNtf rxInfoDebugNtf_twr;
    /** twr vs measurment for each responders  */
    extraTwoWayRangingMesr extraMesr[12];//max num responders
} extraTwoWayRangingData;

/**
 * \brief  Structure lists out the OWR Vendor speicifc measurements for each responder
 */
typedef struct extraOneWayRangingMesr_t
{
    /** AoA / PDoA measurements per RX */
    extraAoaPdoaMesr aoaPdoaMesr_owr[8];
    /** RSSI in dB **/
    int16_t rssi;
} extraOneWayRangingMesr;

/**
 * \brief  Structure lists out the Vendor speicifc information for OWR
 */
typedef struct extraOneWayRangingData_t
{
    /** Vendor Specific Data*/
    /** Rx Antenna Info for AoA Measurements*/
    extraRxInfoMesr rxInfoMesr_owr;
    /** owr vs measurment for each responders  */
    extraOneWayRangingMesr vsMesr[22];
} extraOneWayRangingData;




/**
 * \brief  Structure lists out the TDoA ranging measurement information -- one way ranging.
 */
typedef struct extraRangingMesrTdoa_t
{
    /** Mac address of the participating device,  addr can be
     * of short 2 byte or extended 8 byte modes */
    uint8_t mac_addr[8];
    /** Status */
    uint8_t status;
    /** This signifies the presence of Device ID, Rx and Tx timestamps */
    uint8_t message_control;
    /** Type of frame */
    uint8_t frame_type;
    /** Indicates if the ranging measurement was in Line of sight or
     * non-line of sight */
    uint8_t nLos;
    /** AOA Azimuth */
    int16_t aoa_azimuth;
    /** AOA Azimuth FOM */
    uint8_t aoa_azimuth_FOM;
    /** AOA elevation */
    int16_t aoa_elevation;
    /** AOA elevation FOM */
    uint8_t aoa_elevation_FOM;
    /** Number received in the Payload of the blink UTM from the UT-Tag
     *   or UTM from the UT-Synchronization Anchor. */
    uint32_t frame_number;
    /** Local RX timestamp of the received UWB RFRAME as measured by
     *  the device that is sending this SESSION_INFO_NTF. The unit is
     *  2^(-7) of the 499.2 MHz chipping period, which is
     *  approximately 15.65ps.  */
    uint8_t rx_timestamp[8];
    /** UL-TDoA Device ID of the sender of the received UTM as listed
     *  in the Blink UTM from the UT-Tag or the Synchronization UTM
     *  from the UT-Synchronization Anchor. */
    uint8_t ul_tdoa_device_id[8];
    /** TX timestamp of the UWB RFRAME as listed in the Blink UTM
     *  from the UT-Tag or the Synchronization UTM from UT-Synchronization
     *  Anchor. The unit is 2^(-7) of the 499.2 MHz chipping period,
     *   which is approximately 15.65ps. */
    uint8_t tx_timestamp[8];
} extraRangingMesrTdoa;


typedef struct extraTdoaRangingData_t
{
    /** Vendor Specific Data*/
    /** RSSI RX1 */
    int16_t rssi_rx1;
    /** RSSI RX2 */
    int16_t rssi_rx2;
    /** No of pdoA measurement */
    uint8_t noOfPdoaMeasures;
    /** Estimation of phase difference for antenna pair N */
    int16_t pdoa[8];
    /** CIR Index estimate at which pdoa has been detected */
    uint16_t pdoaIndex[8];
} extraTdoaRangingData;

typedef union {
    /** Ranging measurements array */

    extraTwoWayRangingData twr;
    /** One Way Ranging Ntf*/
    extraOneWayRangingData owr_aoa;
    /** One Way Ranging TDoA Ntf*/
    extraTdoaRangingData tdoa;
} extraMeasurements;

typedef union {
    twr_mesr twr[MAX_RESPONDERS];
    //tdoa_mesr tdoa[MAX_TDOA_MEASURES];
    //dltdoa_mesr dltdoa[MAX_TDOA_MEASURES];
    //owr_aoa_mesr owr_aoa[MAX_OWR_AOA_MEASURES];
} RangingMeasurements;

struct /*__attribute__((packed))*/ RangingResult {
    // Common header fields from phRangingData_t
    uint8_t rcr_indication;
    uint8_t ranging_measure_type;
    uint8_t mac_addr_mode_indicator;  // 0=short(2bytes), 1=extended(8bytes)
    uint8_t no_of_measurements;
    uint8_t antenna_pair_info; // Antenna configuration info
    uint32_t sequence_number;         // maps to seq_ctr
    uint32_t session_handle;
    uint32_t range_interval_ms;       // maps to curr_range_interval
    RangingMeasurements measurements;
    uint8_t antenna_info;
    
};

struct SessionConfig {
    uint32_t session_id;
    SessionType type;
    DeviceRole role;
    DeviceType dev_type;
    RangingMethod ranging_mode;
    MacAddress device_mac;
    uint16_t slots_per_round;
    uint32_t ranging_interval_ms;
};

struct SessionInfo {
    uint32_t sessionHandle;  // Session Handle
    uint8_t state;          // Session state
    uint8_t reason_code;    // Reason code
};

struct AntennaConfig {
    uint8_t tx_antenna;
    uint8_t rx_antenna;
    uint8_t antenna_pair;
};

struct UwbConfig {
    uint8_t channel;          // 5,6,8,9
    uint8_t prf_mode;         // 0=BPRF, 1=HPRF
    uint8_t preamble_code;    // 9-12 for BPRF, 25-32 for HPRF
    uint8_t sfd_id;
    AntennaConfig antenna;
    uint32_t ranging_interval_ms;
};


    
struct AndroidConfigProfile {
    std::array<uint8_t, 2> spec_version_major;
    std::array<uint8_t, 2> spec_version_minor;
    uint32_t session_id;
    uint8_t preamble_id;
    uint8_t channel;
    uint8_t profile_id;
    uint8_t ranging_role;
    std::array<uint8_t, 2> phone_mac_address;
};

enum ProfileId : uint8_t {
    Profile_1 = 0x0B
    
};
struct ProfileInfo {
    uint32_t session_handle;
    uint8_t mac_addr[2];  
    uint8_t profile_id;
    DeviceRole device_role;
    DeviceType device_type;
};

typedef struct AppParams_value_au8
{
    uint8_t *param_value;
    uint16_t param_len; // Just to  handle parameter of any length
} AppParamValue_au8;

typedef union AppParams_value_t {
    uint32_t vu32; // All values u8, u16 and u32 are processed here
    AppParamValue_au8 au8;
} AppParamValue;

typedef struct VendorAppConfig_t {
    VendorAppConfigId param_id;
    AppParamType param_type;
    AppParamValue param_value;
} VendorAppConfig;

typedef struct AppConfig_t {
    AppConfigId param_id;
    AppParamType param_type;
    AppParamValue param_value;
    
} AppConfig;

typedef struct Debug_Params_value_t
{
    uint8_t *param_value;
    uint16_t param_len; // Just to  handle parameter of any length
} Debug_Param_value;
/**
 *  \brief Set/Get Debug Configuration parameters value structure supported in UWB API layer.
 */
typedef union UWB_DebugParams_value {
    uint8_t vu8;   //values u8 are processed here
    uint16_t vu16; //values u16 are processed here
    uint32_t vu32; //values u32 are processed here
    Debug_Param_value param;
} DebugParamValue;

typedef struct DebugConfig_t {
    uint8_t param_id;
    uint8_t param_type;
    DebugParamValue param_value;
} DebugConfig;

typedef struct AndroidDeviceConfig_t {
    uint8_t *config_data;           // Input config data
    uint16_t config_data_length;   // Length of config data
    struct ProfileInfo profile_info;       // Output profile info 
    std::vector<VendorAppConfig> vendor_configs;  // Optional vendor configs
    std::vector<DebugConfig> debug_configs;       // Optional debug configs
} AndroidDeviceConfig;

struct DeviceInfo {
    uint8_t macMajorVersion;                    // MAC major version
    uint8_t macMinorMaintenanceVersion;         // MAC minor/maintenance version
    uint8_t phyMajorVersion;                    // PHY major version
    uint8_t phyMinorMaintenanceVersion;         // PHY minor/maintenance version
    uint8_t devNameLen;                         // Device name length
    uint8_t devName[48];                        // Device name
    uint8_t fwMajor;                           // Firmware major version
    uint8_t fwMinor;                           // Firmware minor version
    uint8_t fwRc;                              // Firmware RC version
    uint8_t nxpUciMajor;                       // NXP UCI major version
    uint8_t nxpUciMinor;                       // NXP UCI minor version
    uint8_t nxpUciPatch;                       // NXP UCI patch version
    uint8_t nxpChipId[16];                     // NXP chip ID (MAX_UWB_CHIP_ID_LEN = 16)
    uint8_t maxPpmValue;                       // Max PPM value
    int16_t txPowerValue[2];                   // TX power value (MAX_TX_POWER_LEN = 2)
    uint8_t mwMajor;                           // MW major version
    uint8_t mwMinor;                           // MW minor version
    uint8_t mwRc;                              // MW RC version
    uint8_t uciGenericMajor;                   // UCI generic major version
    uint8_t uciGenericMinorMaintenanceVersion; // UCI generic minor/maintenance version
    uint8_t uciGenericPatch;                   // UCI generic patch version
    uint8_t uciTestMajor;                      // UCI test major version
    uint8_t uciTestMinor;                      // UCI test minor version
    uint8_t uciTestPatch;                      // UCI test patch version
    uint8_t fwBootMode;                        // Firmware boot mode
#if UWBFTR_CCC
    uint8_t uciCccVersion[2];                  // UCI CCC version (MAX_UCI_CCC_VERSION_LEN = 2)
    uint8_t cccVersion[8];                     // CCC version (MAX_CCC_VERSION_LEN = 8)
#endif
};

struct ProfileConfig {
    std::vector<uint8_t> sharable_data;
    ProfileInfo profile_info;
    std::vector<AppConfig> app_configs;
    //std::vector<VendorAppConfig> vendor_configs;
    //std::vector<DebugConfig> debug_configs;
};


//Android device configuration
typedef struct DeviceConfig_t {
    uint8_t spec_version_major[2];
    uint8_t spec_version_minor[2];
    uint8_t chip_id[2];
    uint8_t chip_fw_version[2]; 
    uint8_t mw_version[3];
    uint32_t supported_profiles;
    uint8_t ranging_role;
    uint8_t device_mac_addr[2]; // Short MAC address
} DeviceConfig;

//iOS device configuration
struct AccessoryConfigData {
    uint8_t length;
    uint8_t spec_version_major[2];
    uint8_t spec_version_minor[2];
    uint8_t manufacturer_id[4];
    uint8_t model_id[4];
    uint8_t mw_version[4];
    DeviceRole ranging_role;
    uint8_t device_mac_addr[2];
    uint8_t clock_drift[2];
};

enum PreferredUpdateRate
{
    Automatic       = 0,
    Infrequent      = 10,
    UserInteractive = 20
};
struct iOSAccessoryConfigData {
    uint8_t customer_spec_major_ver[2];   // Customer specification major version
    uint8_t customer_spec_minor_ver[2];   // Customer specification minor version
    PreferredUpdateRate preferred_update_rate;        // Preferred update rate
    uint8_t rfu[10];                     // Reserved for future use
    AccessoryConfigData uwb_config_data;    // UWB configuration data
};



typedef struct DeviceCapabilities_t {
    // FIRA PHY version range
    uint8_t firaPhyLowerRangeMajorVersion;
    uint8_t firaPhyLowerRangeMinorMaintenanceVersion;
    uint8_t firaPhyHigherRangeMajorVersion;
    uint8_t firaPhyHigherRangeMinorMaintenanceVersion;
    
    // FIRA MAC version range
    uint8_t firaMacLowerRangeMajorVersion;
    uint8_t firaMacLowerRangeMinorMaintenanceVersion;
    uint8_t firaMacHigherRangeMajorVersion;
    uint8_t firaMacHigherRangeMinorMaintenanceVersion;
    
    // Device capabilities
    uint8_t deviceTypes;
    uint16_t deviceRoles;
    uint16_t rangingMethod;
    uint8_t stsConfig;
    uint8_t multiNodeMode;
    uint8_t rangingTimeStruct;
    uint8_t scheduledMode;
    uint8_t hoppingMode;
    uint8_t blockStriding;
    uint8_t uwbInitiationTime;
    uint8_t channels;
    uint8_t rframeConfig;
    uint8_t ccConstraintLength;
    uint8_t bprfParameterSets;
    uint8_t hprfParameterSets[5];  // DEVICE_CAPABILITY_LEN_5
    uint8_t aoaSupport;
    uint8_t extendedMacAddress;
    uint8_t suspendRanging;
    uint8_t sessionKeyLen;
    uint8_t ancorMaxRrActive;
    uint8_t tagMaxRrActive;
    uint16_t maxMessageSize;
    uint16_t maxDataPacketPayloadSize;

#if UWBIOT_UWBD_SR1XXT_SR2XXT
    uint8_t slotBitmask;
    uint8_t syncCodeIndexBitmask[4];  // DEVICE_CAPABILITY_LEN_4
    uint8_t hoppingConfigBitmask;
    uint8_t channelBitmask;
    uint16_t supportedProtocolVersion;
    uint16_t supportedUWBConfigID;
    uint8_t supportedPulseShapeCombo[9];  // DEVICE_CAPABILITY_LEN_9
    uint16_t maxUciPayloadLength;
    uint8_t inbandDataBlockSize;
    uint8_t inbandDataMaxBlock;
    uint8_t tagBlockSkipping;
    uint8_t psduLengthSupport;
#endif //UWBIOT_UWBD_SR1XXT_SR2XXT
} DeviceCapabilities;

typedef struct RangingParams_t {
    uint8_t device_role;              // Role of device (controller/controlee)
    uint8_t device_type;              // Type of device
    uint8_t ranging_mode;             // Mode of ranging (DS-TWR, SS-TWR, etc)
    uint8_t multi_node_mode;          // Multi-node mode configuration
    uint8_t channel_number;           // UWB channel number
    uint8_t prf_mode;                // Pulse Repetition Frequency mode
    uint8_t preamble_code_index;     // Preamble code index
    uint8_t sfd_id;                  // Start Frame Delimiter ID
    uint8_t psdu_data_rate;          // PSDU data rate
    uint8_t mac_fcs_type;           // MAC FCS type
    uint16_t ranging_interval;       // Interval between ranging attempts (ms)
    uint16_t slots_per_round;       // Number of slots per ranging round
    uint8_t max_rr_retry;           // Max ranging round retry attempts
    uint16_t uwb_initiation_time;   // Time before UWB initiation (us)
    uint8_t slot_duration;          // Duration of each slot (ms)
    uint8_t rframe_config;          // Ranging frame configuration
    uint8_t cap_size_range;         // Capability size range
    uint8_t schedule_mode;          // Scheduling mode
    uint32_t key_rotation;          // Key rotation interval
    uint8_t key_rotation_rate;      // Rate of key rotation
    uint8_t priority;               // Session priority level
    
    // Optional parameters
    bool hopping_mode;              // Channel hopping enabled/disabled
    bool sts_config;                // STS configuration enabled/disabled
    bool measurement_report;        // Measurement reporting enabled/disabled
    bool ranging_round_control;     // Ranging round control enabled/disabled
} RangingParams;

struct HusControllerConfig {
    HusConfigMode config_mode;            // Configuration mode
    HusSecurityMode security_mode;        // Security configuration
    uint8_t channel_number;               // UWB channel to use
    uint8_t preamble_code_index;          // Preamble code index
    uint16_t slots_per_ranging_round;     // Number of slots per round
    uint32_t ranging_interval_ms;         // Interval between ranging attempts
    uint8_t mac_fcs_type;                // MAC FCS type
    uint8_t ranging_round_usage;         // Ranging round configuration
    uint8_t sts_config;                  // STS configuration
    uint8_t multi_node_mode;             // Multi-node mode settings
    uint8_t channel_number_list[4];      // List of supported channels
    uint8_t number_of_controlees;        // Number of connected controlees
    std::vector<MacAddress> controlee_addresses;  // MAC addresses of controlees
    
    // Optional parameters
    bool hopping_mode_enabled;           // Channel hopping enabled
    bool measurement_report_enabled;      // Enable measurement reporting
    bool ranging_diagnostics_enabled;     // Enable diagnostic data
    
    // Advanced timing parameters
    uint16_t slot_duration_ms;           // Duration of each slot
    uint16_t ranging_round_timeout_ms;   // Timeout for ranging rounds
    uint8_t max_ranging_round_retries;   // Maximum retry attempts
    
    // Session specific parameters
    uint32_t session_key;                // Session encryption key
    uint8_t session_priority;            // Session priority level
    uint32_t key_rotation_rate;          // Key rotation interval
    
    HusControllerConfig() : 
        config_mode(HusConfigMode::HUS_UNICAST),
        security_mode(HusSecurityMode::HUS_NO_SECURITY),
        channel_number(9),               // Default channel
        preamble_code_index(10),         // Default preamble
        slots_per_ranging_round(12),     // Default slots
        ranging_interval_ms(100),        // 100ms default interval
        mac_fcs_type(0),
        ranging_round_usage(0),
        sts_config(0),
        multi_node_mode(0),
        number_of_controlees(0),
        hopping_mode_enabled(false),
        measurement_report_enabled(true),
        ranging_diagnostics_enabled(false),
        slot_duration_ms(2),
        ranging_round_timeout_ms(50),
        max_ranging_round_retries(3),
        session_key(0),
        session_priority(0),
        key_rotation_rate(0)
    {
        std::fill(channel_number_list, channel_number_list + 4, 0);
    }
};

struct HusControleeConfig {
    HusConfigMode config_mode;            // Configuration mode
    HusSecurityMode security_mode;        // Security configuration
    MacAddress controller_address;        // Controller MAC address
    uint8_t channel_number;               // UWB channel to use
    uint8_t preamble_code_index;          // Preamble code index
    uint8_t slot_index;                   // Assigned slot index
    uint16_t slots_per_ranging_round;     // Number of slots per round
    uint32_t ranging_interval_ms;         // Interval between ranging attempts
    
    // Configuration flags
    uint8_t mac_fcs_type;                // MAC FCS type
    uint8_t sts_config;                  // STS configuration
    uint8_t multi_node_mode;             // Multi-node mode settings
    uint8_t ranging_round_usage;         // Ranging round configuration
    
    // Optional parameters
    bool hopping_mode_enabled;           // Channel hopping enabled
    bool measurement_report_enabled;      // Enable measurement reporting
    bool ranging_diagnostics_enabled;     // Enable diagnostic data
    bool listen_only_mode;               // Listen-only mode enabled
    
    // Timing parameters
    uint16_t slot_duration_ms;           // Duration of each slot
    uint16_t ranging_round_timeout_ms;   // Timeout for ranging rounds
    uint8_t max_ranging_round_retries;   // Maximum retry attempts
    uint16_t response_delay_us;          // Response delay in microseconds
    
    // Security parameters
    uint32_t session_key;                // Session encryption key
    uint8_t session_priority;            // Session priority level
    uint32_t key_rotation_rate;          // Key rotation interval
    
    HusControleeConfig() : 
        config_mode(HusConfigMode::HUS_UNICAST),
        security_mode(HusSecurityMode::HUS_NO_SECURITY),
        channel_number(9),               // Default channel
        preamble_code_index(10),         // Default preamble
        slot_index(0),
        slots_per_ranging_round(12),     // Default slots
        ranging_interval_ms(100),        // 100ms default interval
        mac_fcs_type(0),
        sts_config(0),
        multi_node_mode(0),
        ranging_round_usage(0),
        hopping_mode_enabled(false),
        measurement_report_enabled(true),
        ranging_diagnostics_enabled(false),
        listen_only_mode(false),
        slot_duration_ms(2),             // 2ms default
        ranging_round_timeout_ms(50),    // 50ms default
        max_ranging_round_retries(3),
        response_delay_us(500),          // 500us default
        session_key(0),
        session_priority(0),
        key_rotation_rate(0)
    {
        controller_address.is_short = true;
        controller_address.addr.fill(0);
    }
};

struct DataTransferPhaseConfig {
    uint8_t phase_id;                  // Phase identifier
    uint8_t protocol_version;          // Protocol version to use
    uint8_t data_transfer_type;        // Type of data transfer (unicast/multicast)
    uint16_t data_repetition_count;    // Number of times to repeat data
    uint16_t data_payload_length;      // Length of data payload in bytes
    uint16_t mtu_size;                // Maximum transmission unit size
    uint16_t inter_frame_interval;     // Interval between frames (us)
    uint8_t block_striding_len;       // Block striding length
    uint8_t round_index;              // Round index for this phase
    uint8_t number_of_rounds;         // Total number of rounds
    
    // Security parameters
    uint8_t security_mode;            // Security mode for data transfer
    uint32_t session_key;             // Session encryption key
    uint8_t key_rotation_rate;        // Key rotation rate
    
    // Optional configurations
    bool enable_ranging;              // Enable ranging during data transfer
    bool enable_hopping;             // Enable channel hopping
    bool enable_diagnostics;         // Enable diagnostic reporting
    
    // Timing parameters
    uint16_t slot_duration_us;        // Duration of each slot
    uint16_t phase_timeout_ms;        // Phase timeout
    uint8_t max_retry_count;         // Maximum retry attempts
    
    DataTransferPhaseConfig() :
        phase_id(0),
        protocol_version(0x01),
        data_transfer_type(0),
        data_repetition_count(1),
        data_payload_length(0),
        mtu_size(MAX_APP_DATA_SIZE),
        inter_frame_interval(1000),    // 1ms default
        block_striding_len(1),
        round_index(0),
        number_of_rounds(1),
        security_mode(0),
        session_key(0),
        key_rotation_rate(0),
        enable_ranging(false),
        enable_hopping(false),
        enable_diagnostics(false),
        slot_duration_us(2000),        // 2ms default
        phase_timeout_ms(100),         // 100ms default
        max_retry_count(3)
    {}
};

struct MulticastControlee {
    MacAddress address;              // Controlee MAC address
    uint8_t slot_index;             // Assigned slot index
    uint8_t sub_session_id;         // Sub-session identifier
    bool active;                    // Controlee active status
};

struct MulticastControleeList {
    std::vector<MulticastControlee> controlees;  // List of controlees
    uint8_t max_controlees;                      // Maximum number of controlees supported
    uint8_t active_controlees;                   // Number of currently active controlees
    uint8_t group_id;                           // Multicast group identifier
    
    MulticastControleeList() :
        max_controlees(MAX_RESPONDERS),
        active_controlees(0),
        group_id(0)
    {}

    // Helper methods
    bool addControlee(const MulticastControlee& controlee) {
        if (controlees.size() >= max_controlees) {
            return false;
        }
        controlees.push_back(controlee);
        if (controlee.active) {
            active_controlees++;
        }
        return true;
    }

    bool removeControlee(const MacAddress& address) {
        for (size_t i = 0; i < controlees.size(); i++) {
            const auto& c = controlees[i];
            bool match = true;
            size_t len = c.address.is_short ? 2 : 8;
            
            for (size_t j = 0; j < len; j++) {
                if (c.address.addr[j] != address.addr[j]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                if (controlees[i].active) {
                    active_controlees--;
                }
                controlees.erase(controlees.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool setControleeStatus(const MacAddress& address, bool active) {
        for (size_t i = 0; i < controlees.size(); i++) {
            const auto& c = controlees[i];
            bool match = true;
            size_t len = c.address.is_short ? 2 : 8;
            
            for (size_t j = 0; j < len; j++) {
                if (c.address.addr[j] != address.addr[j]) {
                    match = false;
                    break;
                }
            }
            
            if (match) {
                if (controlees[i].active != active) {
                    active_controlees += (active ? 1 : -1);
                    controlees[i].active = active;
                }
                return true;
            }
        }
        return false;
    }
};


union DeviceConfigValue {
    AntennaConfig antenna;                // For ANTENNA_CONFIG
    struct {
        uint8_t mode;                     // Sleep/idle/active
        uint16_t wakeup_interval_ms;      // Wake-up interval
    } power;                             // For LOW_POWER_MODE
    struct {
        uint8_t sync_mode;               // Clock synchronization mode
        uint32_t sync_timeout_us;        // Sync timeout in microseconds
    } clock;                            // For CLOCK_CONFIG
    struct {
        uint8_t channel;                 // UWB channel number
        uint8_t prf_mode;               // Pulse repetition frequency mode
        uint8_t data_rate;              // Data rate configuration
    } channel;                         // For CHANNEL_CONFIG
    struct {
        RangingMethod default_mode;        // Default ranging mode
        uint32_t default_interval_ms;    // Default ranging interval
    } ranging;                         // For RANGING_CONFIG
    struct {
        uint8_t sts_mode;               // STS mode configuration
        uint8_t key_rotation_rate;      // Key rotation rate
    } sts;                            // For STS_CONFIG
    struct {
        uint8_t wake_mode;              // Wake-up mode
        uint16_t wake_period_ms;        // Wake period in milliseconds
    } wakeup;                         // For WAKEUP_CONFIG
    struct {
        int8_t tx_power_dbm;            // TX power in dBm
        uint8_t fine_gain;              // Fine gain adjustment
    } tx_power;                       // For TX_POWER
    struct {
        int8_t rx_threshold_dbm;        // RX threshold in dBm
        uint8_t preamble_detect;        // Preamble detection threshold
    } rx_sens;                        // For RX_SENSITIVITY
    struct {
        uint32_t calib_flags;           // Calibration flags
        std::array<uint8_t, 32> data;   // Calibration data
    } calibration;                    // For CALIBRATION
    struct {
        uint8_t debug_level;            // Debug logging level
        uint32_t debug_flags;           // Debug feature flags
    } debug;                          // For DEBUG_MODE
    std::vector<uint8_t> vendor_data;   // For VENDOR_SPECIFIC
};

struct DeviceConfigData {
    DeviceConfigId config_id;           // Configuration identifier
    DeviceConfigValue value;            // Configuration value
    
    
};


/**
 * \brief  Enumeration lists out the STS Config values.
 */
enum  StsConfig : uint8_t
{
    StaticSts               = 0,
    DynamicSts              = 1,
    DynamicSts_Ctrlee_key   = 2,
    ProvisionSts            = 3,
    ProvisionSts_Ctrlee_key = 4,

} ;



struct RangingConfig {
    DeviceRole device_role;          // Role of the device in the ranging session
    DeviceType device_type;          // Type of device (controller/controlee/etc)
    MultiNodeMode multi_node_mode;   // Single or multi-node operation mode
    uint8_t mac_addr_mode;          // MAC address mode (0=2bytes, 2=8bytes)
    uint8_t device_mac_addr[8];     // Device MAC address (up to 8 bytes)
    RangingMethod ranging_method;    // Method used for ranging measurements
    ScheduledMode scheduled_mode;    // Scheduling mode for ranging rounds
    uint8_t no_of_controlees;        // Number of controlees in the session
    uint8_t dst_mac_addr[12 * MAC_EXT_ADD_LEN];
};

// Callback function types
using RangingCallback = void (*)(const RangingResult& result);
using DataCallback = void (*)(const uint8_t* data, size_t len);
using ErrorCallback = void (*)(Status status, const char* message);
using PrintCallback = void (*)(const char *szString);
using SystemNotificationCallback = void (*)(uwb::NotificationType, void *);


struct DataTransmit {
    uint32_t transmitNtf_sessionHandle;     // Session Handle
    uint16_t transmitNtf_sequence_number;   // Sequence number
    uint8_t transmitNtf_status;            // Status
    uint8_t transmitNtf_txcount;           // Tx count

    // Constructor with default initialization
    DataTransmit() :
        transmitNtf_sessionHandle(0),
        transmitNtf_sequence_number(0),
        transmitNtf_status(0),
        transmitNtf_txcount(0)
    {}
};

struct DataPacket {
    uint32_t session_handle;            // Session Handle
    uint8_t mac_address[MAC_EXT_ADD_LEN];  // MAC Address 
    uint16_t sequence_number;           // Sequence Number
    uint16_t data_size;                // Data Size
    uint8_t* data;                     // Pointer to Application Data

    // Constructor with default initialization
    DataPacket() :
        session_handle(0),
        sequence_number(0),
        data_size(0),
        data(nullptr)
    {
        memset(mac_address, 0, MAC_EXT_ADD_LEN);
    }
};

struct GenericError
{
    /** Status */
    uint8_t status;
} ;

enum RfFrameConfig : uint8_t
{
    No_Sts               = 0,
    SP0                  = No_Sts,
    Sfd_Sts              = 1,
    SP1                  = Sfd_Sts,
    Psdu_Sts             = 2,
    Sfd_Sts_NoPhr_NoPsdu = 3,
    SP3                  = Sfd_Sts_NoPhr_NoPsdu,
} ;


/**
 *  \brief Macro to set SetApp Configuration parameters value supported in UWB API layer.
 */
#define SET_APP_PARAM_VALUE(PARAM, VALUE)                                                      \
    {                                                                                              \
        .param_id = (PARAM), .param_type = (uwb::AppParamType::U32), .param_value={.vu32 = (VALUE)}, \
    }

/**
 *  \brief Macro to set GetApp Configuration parameters value supported in UWB API layer.
 */
#define SET_GETAPP_PARAM(PARAM)                                   \
    {                                                                 \
        .param_id = (PARAM), .param_type = (uwb::AppParamType::U32), \
    }

/**
 *  \brief Macro to set SetApp/GetApp Configuration parameters array value supported in UWB API layer.
 */
#define SET_APP_PARAM_ARRAY(PARAM, ARRAY, LENGTH)\
    {\
        .param_id = (PARAM), .param_type = (uwb::AppParamType::ARRAY_U8), .param_value =  \
        { .au8 = { .param_value = (uint8_t *)(ARRAY),.param_len = (LENGTH)}}  \
    }

} // namespace uwb