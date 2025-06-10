// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBAppParamsList.hpp"
#include "UWBMacAddress.hpp"


#ifndef __UWBAPPPARAMLIST_HPP__
#define __UWBAPPPARAMLIST_HPP__
uwb::AppConfig buildScalar(uwb::AppConfigId id, uint32_t val);
uwb::AppConfig buildArray(uwb::AppConfigId id, uint8_t *val, uint8_t length);
class UWBAppParamList : public UWBAppParamsList<uwb::AppConfig, uwb::AppConfigId, uwb::AppParamType, uwb::AppParamValue>
{
public:
     /**
      * @brief Set the UWB channel. Accepted values are 5 (6.5Ghz) and 9 (8 Ghz)
      *
      * @param[in] channelNo   UWB channel number
      *
      * @return true                 on success
      * @return false                on failure
      */
     bool channel(uint32_t channelNo);

     /**
      * @brief Set the index for the UWB Preamble code.
      * the Preamble is a known sequence that is transmitted at the beginning of a
      * packet or frame. It helps the receiver synchronize with the incoming signal,
      * compensating for factors like timing offsets and frequency misalignments.
      *
      * @param[in] pci Preamble Code Index
      * valid values:
      * kUWB_PreambleIndxCode_BPRF_09 = 9
      * kUWB_PreambleIndxCode_BPRF_10 = 10,
      * kUWB_PreambleIndxCode_BPRF_11 = 11,
      * kUWB_PreambleIndxCode_BPRF_12 = 12,
      *
      * @return true                 on success
      * @return false                on failure
      */
     bool preambleCodeIndex(uint32_t pci);

     /**
      * @brief Set the SFD ID for the session.
      * Start-of-Frame Delimiter (SFD): A specific sequence indicating the start
      * of the actual data frame.
      * This library supports pattern #0 (32 bits) and pattern #2 (64 bits)
      *
      * @param[in] sfdId SFD ID
      * valid values:
      * kUWB_SfdId_BPRF_0 = 0,
      * kUWB_SfdId_BPRF_2 = 2,
      *
      * @return true                 on success
      * @return false                on failure
      */
     bool sfdId(uint32_t sfdId);

     /**
      *
      *
      * A UWB Session in Time Schedule mode has timings configured according to
      *  the following diagram:
      *
      * |                                                                      |
      * |      RANGING_DURATION (one full Ranging Round with multiple slots)   |
      * |<-------------------- RANGING_DURATION ------------------------------>|
      * |                                                                      |
      * | SLOT 1       SLOT 2       SLOT 3        ...       SLOT #SLOTS_PER_RR |
      * |<------->    <------->    <------->               <------->           |
      * |  SLOT_DURATION                                                       |
      * |                                                                      |
      * |                                                                      |
      * | Slot #1      Slot #2      Slot #3       ...      Slot #SLOTS_PER_RR  |
      *
      * Where:
      *
      *
      * RANGING_DURATION represents the time taken to complete one full Ranging
      * Round i.e. an entire range-measuremet cycle.
      * Within this duration, multiple slots are allocated for ranging
      * transaction.
      *
      *
      * SLOTS_PER_RR is the number of slots in a ranging round. Each slot has a
      * specific duration, and devices are assigned to one or more slots during
      * the ranging round. Depending on the ranging mode one may need to allocate
      * a certain minimum of slots per ranging round.
      * For example, in a Double-Sided Two-way-Ranging Session 6 messages are
      * exchanged so at least 6 slots are needed.
      * Normally one will allocate even more slots per ranging round to allow for
      * retransmissions or multiple devices.
      *
      *
      *
      * SLOT_DURATION is the duration of a single slot. In high speed configuration
      * this is typically 1 or 2 milliseconds. Longer durations may be useful for
      * configurations where it may take some time to process data between transmissions
      */

     /*
      * @brief Set the Ranging Duration parameter, in milliseconds
      *
      * @param duration , default value is 200
      * @return true
      * @return false
      */
     bool rangingDuration(uint32_t duration);

     /**
      * @brief Set the Slots Per Ranging Round parameter
      *
      * @param slots
      * @return true
      * @return false
      */

     bool slotPerRR(uint32_t slots);

     /**
      * @brief Set the Slot Duration parameter
      * in RSTUs where 1200 RSTU == 1 millisecond
      * Default value is 2400
      *
      * @param duration
      * @return true
      * @return false
      */
     bool slotDuration(uint32_t duration);

     /**
      * @brief Set the STS Configuration for the session.
      * Scrambled Timestamp Sequence is a securitized data block that is
      * essential to provide resiliency to UWB attacks.
      *
      * The STS Config defines which method is used to generate and use the crypto
      * materials used to scramble timestamps (and data)
      *
      * @param[in] sts STS configuration
      * valid values:
      * kUWB_StsConfig_StaticSts               = 0, // Use static key, default
      *
      * kUWB_StsConfig_DynamicSts              = 1, // STS changes on a per slot
      * 									     basis. A Secure Element
      * 										generates the crypto material
      * kUWB_StsConfig_DynamicSts_Ctrlee_key   = 2,
      *
      * kUWB_StsConfig_ProvisionSts            = 3,
      * In a UWB system, the Provisioned STS is a sequence that is pre-configured
      * (or "provisioned") onto each device before communication. This sequence is
      * unique to a device or group of devices and serves as a secure identifier in
      * UWB ranging and communication sessions.
      * This provisioned STS is shared only among authorized devices, preventing
      * unauthorized devices from participating or impersonating others in the
      * session.
      *
      * kUWB_StsConfig_ProvisionSts_Ctrlee_key = 4,
      *
      * @return true                 on success
      * @return false                on failure
      */
     bool stsConfig(uint32_t sts);

     /**
      * @brief set the number of STS segments
      *
      * depending on the STS Congiguration one can specify 0 segments (for SP0)
      * or 1 segment (for SP1 and SP3)
      *
      * Default is 1
      *
      * @param segments number of segments
      * @return true
      * @return false
      */
     bool stsSegments(uint8_t segments);

     /**
      * @brief Set the RFRAME STS Packet configuration
      *
      * In UWB Systems a RFRAME can support different STS Packet profiles (SP).
      * The most relevant ones are SP0, SPI1 and SP3
      *
      *
      * SP0 (STS Packet 0):
      *     The SP0 frame structure is:
      *
      * +-------------------------------------------+
      * |           SP0 RFRAME Structure            |
      * +-------------------------------------------+
      * | Preamble | SFD  | Header | Data Payload   |
      * +-------------------------------------------+
      *
      *     Description: SP0 is a non-secure configuration, meaning it provides
      *     basic ranging functionality without encryption or message authentication.
      *     Components: The RFRAME contains only essential information for ranging,
      *     such as the preamble, synchronization sequence, and basic ranging data.
      *
      *     Use Case: SP0 is used in scenarios where security is not a primary
      *     concern, or where power consumption and simplicity are prioritized,
      *     such as in indoor positioning systems where the environment is
      *     controlled and unauthorized access is unlikely.
      *
      *     Advantages: Minimal overhead due to the absence of security features,
      *     leading to lower power consumption and faster processing.
      *     Disadvantages: Vulnerable to eavesdropping, replay attacks, and
      *     tampering, making it unsuitable for secure applications.
      *
      * SP1 (STS Packet 1):
      *     The SP1 frame structure is:
      *
      * +-----------------------------------------------+
      * |              SP1 RFRAME Structure             |
      * +-----------------------------------------------+
      * | Preamble | SFD | Header | STS  | Data Payload |
      * +-----------------------------------------------+
      *
      *
      *     Description: SP1 provides data integrity and message authentication.
      *     It ensures that messages cannot be easily altered or forged. It
      *     carries a payload
      *
      *
      *     Use Case: SP1 is suitable for applications that perform data
      *     transmission
      *
      *
      * SP3 (STS Packet 3):
      *
      * +-----------------------------------------------------+
      * |               SP3 RFRAME Structure                  |
      * +-----------------------------------------------------+
      * | Preamble | SFD | STS (Scrambled Timestamp Sequence) |
      * +-----------------------------------------------------+
      *
      *
      *     Description:  the main goal in this profile is to protect the timing
      *     and integrity of the ranging process without necessarily carrying
      *     additional data in the payload. This configuration ensures
      *     confidentiality, integrity, and protection against relay attacks,
      *     focusing solely on secure ranging rather than data transmission.
      *
      *     Use Case: SP3 is used in applications that require high security,
      *     such as financial transactions, sensitive asset tracking, or secure
      *     access control where both confidentiality and integrity are crucial.
      *
      *     Advantages: Provides comprehensive protection against eavesdropping,
      *     tampering, and replay attacks, making it suitable for security-critical
      *     applications.
      *
      *     Disadvantages: Higher processing and power overhead due to encryption
      *     and authentication operations, potentially increasing latency and
      *     reducing battery life in power-sensitive devices.
      *
      *
      * @param config   	kUWB_RfFrameConfig_No_Sts               = 0,
      *     				kUWB_RfFrameConfig_SP0                  = kUWB_RfFrameConfig_No_Sts,
      * 					kUWB_RfFrameConfig_Sfd_Sts              = 1,
      * 					kUWB_RfFrameConfig_SP1                  = kUWB_RfFrameConfig_Sfd_Sts,
      * 					kUWB_RfFrameConfig_Psdu_Sts             = 2,
      * 					kUWB_RfFrameConfig_Sfd_Sts_NoPhr_NoPsdu = 3,
      *					kUWB_RfFrameConfig_SP3                  = kUWB_RfFrameConfig_Sfd_Sts_NoPhr_NoPsdu,
      *
      * @return true on success
      * @return false on failure
      */

     bool frameConfig(uint8_t config);

     /**
      * @brief set the Ranging Round Usage
      *
      * the RANGING_ROUND_USAGE parameter allows you to select the specific type
      * of ranging method or protocol used in a UWB session. Each of these
      * ranging methods has unique characteristics and applications, enabling
      * devices to tailor their ranging rounds for specific scenarios, accuracy
      * requirements, or energy efficiency.
      *
      * Most common values are TDoA, SS-TWR, DS-TWR
      *
      * @param rru	    kUWB_RangingRoundUsage_TDoA      = 0,
      *                  kUWB_RangingRoundUsage_SS_TWR    = 1,
      *                  kUWB_RangingRoundUsage_DS_TWR    = 2,
      *                  kUWB_RangingRoundUsage_SS_TWR_nd = 3, //non-deferred mode
      *                  kUWB_RangingRoundUsage_DS_TWR_nd = 4, //non-deferred mode
      *                  kUWB_RangingRoundUsage_DL_TDOA   = 5,
      *                  kUWB_RangingRoundUsage_OWR_AOA   = 6,
      *                  kUWB_RangingRoundUsage_eSS_TWR   = 7,
      *                  kUWB_RangingRoundUsage_aDS_TWR   = 8,
      *
      * @return true
      * @return false
      */
     bool rangingRoundUsage(uint8_t rru);

     /**
      * @brief Set the Tx Power Id
      *
      * TX Power ID
      * - 0: max power (14db)
      * - 104: least power(-12db)
      *
      * Range 0 to 104: 0.25db per step
      *
      * default is 0
      *
      * @param id 0 to 104
      * @return true
      * @return false
      */
     bool powerId(uint8_t id);

     /**
      * @brief Set the Max number of Ranging Round attempts before stopping the
      * session and put it in the SESSION_IDLE_STATE.
      * if the value is 0 (default) then termination is disabled and the ranging
      * round attempt is infinite
      *
      * @param retries 0 to 65535
      * @return true
      * @return false
      */
     bool maxRetries(uint16_t retries);

     /**
      * @brief set the timestamp mode for an uplink TDoA tag
      *
      * 0x00: TX timestamp shall not be included (default).
      * 0x01: 40-bit TX timestamp shall be included.
      * 0x02: 64-bit TX timestamp shall be included.
      * Values 0x03 to 0xFF = RFU
      *
      * @param mode
      * @return true success
      * @return false fail
      */
     bool uplinkTdoaTimestamp(uint8_t mode);

     /**
      * @brief set an identifier to an uplink TDoA tag
      *
      * @param value array of bytes
      * @param length length of the array
      *
      * @return true     success
      * @return false    fail
      */
     bool tdoaDeviceId(uint8_t value[], uint8_t length);

     /**
      * @brief set the Blink transmission interval for a TDoA Tag or the Sync interval for a TDoA Sync Anchor
      *
      * Given the isolated, connectionless nature of a TDoA session, there is no
      * need to specify the timing in terms of Ranging Round Duration/Slot
      * Duration/Slots per Ranging round and it's just enough to define an
      * interval at which the tag will send a Blink.
      *
      * @param interval in milliseconds
      * @return true
      * @return false
      */
     bool tdoaTxInterval(uint32_t interval);

     /**
      * @brief define the Frame Check Sequence (FCS) type
      *
      * the FCS field in a UWB frame is a CRC that is used for validation of the
      * payload.
      * It can be either a 16 bits CRC (default) or 32 bit CRC
      *
      * @param type      0 for 16 bits CRC
      *                  1 for 32 bits CRC
      * @return true
      * @return false
      */
     bool macFcsType(uint8_t type);
     
    /**
     * @brief Set the Number Of Controlees 
     * 
     * allows the Controller to define how many devices will participate, 
     * improving resource allocation, synchronization, and collision management. 
     * 
     * @param noOfControlees the number of controlees
     */
    bool noOfControlees(uint8_t number);
    
    /**
     * @brief Set the Destination Mac Address for the session
     * 
     * @param addr 
     */
    bool destinationMacAddr(UWBMacAddress &addr);

     // WHAT BELOW NEEDS SOME CLARIFICATION
     //        /**
     //       * @brief Set the Schedule Mode
     //       *
     //       *
     //       * FiRa defines three Schedule Modes for coordinating UWB ranging sessions:
     //       * Contention-Based, Time-Based, and Hybrid. Each mode addresses different
     //       * requirements for flexibility, efficiency, and collision avoidance, tailored
     //       * to the network density and application needs.
     //       *
     //       * 1. Contention-Based Schedule Mode
     //       *    - Description: Devices operate in an ad hoc manner, initiating ranging
     //       *      sessions as needed without a predefined schedule. Ranging occurs on a
     //       *      first-come-first-served basis, and devices contend for the channel.
     //       *    - Operation:
     //       *      * Each device initiates ranging independently.
     //       *      * If multiple devices range simultaneously, they contend for access,
     //       *        with backoff algorithms managing retries.
     //       *    - Benefits:
     //       *      * Flexibility: On-demand ranging makes this mode adaptable for
     //       *        real-time needs.
     //       *      * Low Overhead: No central schedule management, simplifying setup.
     //       *    - Limitations:
     //       *      * Higher Collision Risk: Greater chance of overlapping transmissions
     //       *        in high-density environments.
     //       *      * Less Predictable Timing: Timing may vary due to backoffs and retries.
     //       *    - Use Cases:
     //       *      * Suitable for low-density environments and user-initiated applications
     //       *        like keyless entry or peer-to-peer ranging.
     //       *
     //       * 2. Time-Based Schedule Mode
     //       *    - Description: Devices use a centrally managed schedule with specific
     //       *      time slots, ensuring only one device ranges at a time. This mode
     //       *      avoids collisions and supports high-density applications.
     //       *    - Operation:
     //       *      * A controller assigns time slots to each device in the network.
     //       *      * Devices range only during their designated slots.
     //       *      * Often uses TDMA (Time Division Multiple Access) to prevent collisions.
     //       *    - Benefits:
     //       *      * Collision-Free: Scheduled slots ensure no overlapping transmissions.
     //       *      * Predictable Timing: Structured timing provides reliability and
     //       *        consistency.
     //       *    - Limitations:
     //       *      * Requires Central Coordination: Needs a controller to assign slots,
     //       *        adding setup complexity.
     //       *      * Less Flexible: Immediate, ad hoc ranging is not feasible.
     //       *    - Use Cases:
     //       *      * Ideal for Real-Time Location Systems (RTLS) in high-density settings
     //       *        like warehouses, factories, or stadiums.
     //       *
     //       * 3. Hybrid Schedule Mode
     //       *    - Description: Combines elements of Contention-Based and Time-Based
     //       *      modes, mixing centrally managed slots with ad hoc, on-demand ranging.
     //       *      This allows for both flexibility and coordination.
     //       *    - Operation:
     //       *      * Certain slots are scheduled by a controller, while other slots are
     //       *        open for contention-based ranging.
     //       *      * Allows dynamic slot assignment and adjustments based on network needs.
     //       *    - Benefits:
     //       *      * Balanced Flexibility and Coordination: Supports devices requiring
     //       *        scheduled slots while allowing flexibility for others.
     //       *      * Reduced Collision Risk: Scheduled slots reduce contention.
     //       *    - Limitations:
     //       *      * Moderate Complexity: Managing both scheduled and unscheduled slots
     //       *        adds overhead.
     //       *      * Varied Timing Reliability: Contention-based slots may still face
     //       *        collision risks.
     //       *    - Use Cases:
     //       *      * Suitable for environments with mixed-density requirements, such as
     //       *        event venues or smart buildings, where both critical and flexible
     //       *        devices coexist.
     //       *
     //       * Summary of Schedule Modes
     //       * ------------------------------------------------------------------------------
     //       * | ScheduleMode        | Description                      | Benefits                 | Limitations                  | Use Cases                    |
     //       * |---------------------|----------------------------------|--------------------------|------------------------------|------------------------------|
     //       * | Contention-Based    | Ad hoc, decentralized            | Flexible, low overhead   | Higher collision risk        | Keyless entry, peer-to-peer  |
     //       * |                     |                                  |                          | Less predictable timing      |                              |
     //       * |---------------------|----------------------------------|--------------------------|------------------------------|------------------------------|
     //       * | Time-Based          | Centralized, time-slot based     | Collision-free           | Requires central coordination| High-density RTLS, asset     |
     //       * |                     |                                  | Predictable timing       | Less flexible                | tracking                     |
     //       * |---------------------|----------------------------------|--------------------------|------------------------------|------------------------------|
     //       * | Hybrid              | Mixed scheduled/unscheduled slots| Balanced flexibility     | Moderate complexity          | Event venues, smart buildings|
     //       * |                     |                                  | Reduced collision risk   | Varied timing reliability    |                              |
     //       * ------------------------------------------------------------------------------
     //       *
     //       * In most cases you are going to use the time-base schedule mode
     //       *
     //       * @param mode
     //       */
     //      bool scheduleMode(uint8_t mode) {
     //         return addOrUpdateParam(buildScalar(RANGING_TIME_STRUCT, mode));
     //      }
};

#endif //__UWBAPPPARAMLIST_HPP__
