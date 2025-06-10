// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBSESSION_HPP
#define UWBSESSION_HPP
#include <stdint.h>
#include "hal/uwb_hal.hpp"
#include "hal/uwb_types.hpp"
#include "UWBRangingParams.hpp"
#include "UWBAppParamList.hpp"
#include "UWBVendorParamList.hpp"

/**
 * @brief UWB Session wrapper class
 * A UWB (Ultra-Wideband) Session refers to a defined period during which two or
 * more UWB devices communicate for specific tasks, such as ranging (distance 
 * measurement), secure data exchange, or location tracking. A session 
 * encompasses the entire process of setting up, managing, and terminating 
 * communication between devices, all while ensuring timing, synchronization, 
 * and reliability of the data exchanged
 * 
 * Key Concepts of a UWB Session:
 * 
 *     Session Initialization:
 *         A UWB session begins with device discovery and session setup. During 
 *         this phase, UWB devices (such as a tag and an anchor) identify each 
 *         other and establish the parameters necessary for communication.
 *         This process can happen, for example, over BLE (see the Nearby demo) 
 *         or one can use hardwired values on all participants (the other demos) 
 *         Parameters include session ID (a unique identifier for the session), 
 *         channel number, preamble code, data rate, and security requirements.
 * 
 *     Session Identifier (Session ID):
 *         Each UWB session is assigned a unique Session ID that differentiates it 
 *         from other sessions. This ID ensures that devices communicate only with 
 *         the intended session participants, even in environments where multiple 
 *         UWB sessions are occurring simultaneously.
 * 
 *     Message Exchange and Timing:
 *         UWB sessions involve a series of timed message exchanges, particularly 
 *         in ranging applications. Precise timing and sequence control are vital 
 *         since UWB devices use time-of-flight measurements to calculate 
 *         distances.
 *         In a ranging session, these exchanges typically follow a defined 
 *         protocol, such as Two-Way Ranging (TWR) or Time-Difference of Arrival 
 *         (TDoA), with each message assigned a slot or time period to avoid 
 *         collisions.
 * 
 *     Session Control and Management:
 *         Session control includes mechanisms to ensure that messages follow the 
 *         correct sequence, detect any errors, and manage retries or 
 *         retransmissions as needed.
 *         Sequence control helps track the order of messages to maintain 
 *         synchronization and timing accuracy, which is essential for correct 
 *         ranging results.
 *         Additional control measures may include dynamic adjustments to session 
 *         parameters, such as adapting the data rate or power level based on 
 *         environmental conditions or the presence of other UWB devices.
 * 
 *     Security and Authentication:
 *         UWB sessions often use authentication and encryption to ensure secure 
 *         communication between devices. For example, secure key exchanges or 
 *         cryptographic techniques verify device identities and protect data from 
 *         interception or tampering.
 *         In applications such as keyless entry or secure access, this security 
 *         component is critical for preventing unauthorized access.
 * 
 *     Ranging and Data Exchange:
 *         The primary function of many UWB sessions is ranging—measuring the 
 *         distance between devices. This involves a sequence of message exchanges, 
 *         where precise timestamps are captured and used to calculate the 
 *         time-of-flight, and thereby, the distance.
 *         For data exchange sessions, UWB can transmit additional data along with 
 *         timing information, supporting applications like file sharing or 
 *         device discovery in a secure and synchronized manner.
 * 
 *     Session Termination:
 *         A UWB session ends once the purpose of the session has been fulfilled, 
 *         such as when a location has been established, or data has been exchanged.
 *         Termination may involve specific messages to signal the end of the 
 *         session and ensure all resources are released, which helps reduce power 
 *         consumption and clears the channel for other devices.
 * 
 * 
 * This class implements the basic operations for a Session such as 
 * initialization, deinitialization starting and stopping.
 * 
 * Moreover it takes care of setting up the session parameters via two public 
 * members:
 *  
 * UWBRangingParams rangingParams;
 * UWBAppParamList appParams;
 * 
 * Ranging Parameters are low-level settings that directly control the technical 
 * aspects of the UWB ranging process. These parameters are essential for 
 * determining the behavior and quality of the ranging session, influencing how 
 * distance is measured, synchronized, and processed.
 *
 * Application Parameters are higher-level settings that define how the UWB 
 * session interacts with the application layer and fulfill specific application 
 * requirements. 
 * These parameters help tailor the UWB experience to the needs of the end-user 
 * application, such as security, device management, and data exchange.
 */
class UWBSession
{
public:
    /**
     * @brief Construct a new UWBSession object
     *
     */
    UWBSession();
    
    uint32_t sessionID();
    /**
     * @brief Set the Session ID
     *
     * @param id
     */
    void sessionID(uint32_t id);
    
    /**
     * @brief Set the Session Type
     *
     * @param sessionType
     */
    void sessionType(uwb::SessionType sessionType);
    
    /**
     * @brief Get the Session Type
     *
     * @return eSessionType
     */
    uwb::SessionType sessionType();
    /**
     * @brief sends the session configuration params and initializes the session
     *
     * @return uwb::Status::SUCCESS if OK
     */
    uwb::Status init();
    /**
     * @brief deinit the session
     */
    uwb::Status deInit();

    /**
     * @brief sends a data packet
     *
     * @param pSendData
     * @return uwb::Status::SUCCESS if OK
     */
    
    uwb::Status sendData(uwb::DataPacket& pSendData);
    uwb::Status sendData(uint8_t *sendData, uint8_t len, uint8_t seq_num, uint8_t *dst_mac);

    /**
     * @brief API to get max data size that can be transferred during a single ranging round.
     *
     * @param pQueryDataSize
     * @return uwb::Status::SUCCESS if OK
     */
    // uint8_t queryDataSize(phUwbQueryDataSize_t *pQueryDataSize);
    /**
     * @brief
     *
     * @param nActiveRounds
     * @param macAddressingMode
     * @param roundConfigList
     * @param pNotActivatedRound
     * @return uint8_t
     */

    /**
     * @brief Update the active rounds during the DL-TDoA Session for a initiator or responder device.
     *
     * @param[in]   nActiveRounds      : Number of active rounds
     * @param[in]   macAddressingMode  : MAC addressing mode- 2/8 bytes
     * @param[in]   roundConfigList    : List/array of size nActiveRounds of round index + role tuple
     * @param[out]  pNotActivatedRound : Structure containing list of not activated index which couldn't be activated, in case return code is #uwb::Status::ERROR_ROUND_INDEX_NOT_ACTIVATED
     *
     * @return #uwb::Status::SUCCESS                                               on success
     * @return #uwb::Status::NOT_INITIALIZED                                  if UWB stack is not initialized
     * @return #uwb::Status::SESSION_NOT_EXIST                                if session is not initialized with sessionHandle
     * @return #uwb::Status::ERROR_ROUND_INDEX_NOT_ACTIVATED                  if one or more rounds couldn't be activated
     * @return #uwb::Status::ERROR_NUMBER_OF_ACTIVE_RANGING_ROUNDS_EXCEEDED   one or more given rounds exceed number of rounds available
     * @return #uwb::Status::TIMEOUT                                          if command is timeout
     * @return #uwb::Status::FAILED                                           otherwise
     */
    // uint8_t updateActiveRoundsAnchor(uint8_t nActiveRounds,    UWB_MacAddressMode_t macAddressingMode,      phActiveRoundsConfig_t *roundConfigList,  phNotActivatedRounds_t *pNotActivatedRound);

    /*set uwb app params*/
    /**
     * @brief Sends immediate App config parameter
     *
     * @param param_id parameter to be set
     * @param param_value it's value
     * @return uwb::Status::SUCCESS on success
     */
    uwb::Status appConfig(uwb::AppConfigId param_id, uint32_t param_value);

    /**
     * @brief Enable Ranging Data Notifications different options.
     * @param[in] enableRangingDataNtf   Enable Ranging data notification  0/1/2.
    option 2 is not allowed when ranging is ongoing.
     * @param[in] proximityNear          Proximity Near value valid if
    enableRangingDataNtf sets to 2
    * @param[in] proximityFar           Proximity far value valid if
    enableRangingDataNtf sets to 2
    *
    * @return #uwb::Status::SUCCESS               on success
    * @return #uwb::Status::NOT_INITIALIZED  if UWB stack is not initialized
    * @return #uwb::Status::INVALID_PARAM    if invalid parameters are passed
    * @return #uwb::Status::FAILED           otherwise
    * @return #uwb::Status::TIMEOUT          if command is timeout
    */
   uwb::Status enableRangingDataNtf(uint8_t enableRangingDataNtf = 1, uint16_t proximityNear = 0, uint16_t proximityFar = 0);
    /**
     * @brief Set the Static Sts Inititialization vector
     *
     * @param vendorId
     * @param staticStsIv
     * @return uint8_t
     */
    uwb::Status staticSts(uint16_t vendorId, uint8_t *staticStsIv);
    /**
     * \brief Get Session State
     *
     * @param[out] sessionState   Session Status
     *
     * @return #uwb::Status::SUCCESS                on success
     * @return #uwb::Status::NOT_INITIALIZED   if UCI stack is not initialized
     * @return #uwb::Status::INVALID_PARAM     if invalid parameters are passed
     * @return #uwb::Status::FAILED            otherwise
     * @return #uwb::Status::TIMEOUT           if command is timeout
     *
     * if API returns uwb::Status::SUCCESS, Session State would be one of the below
     * values
     *  #UWBAPI_SESSION_INIT_SUCCESS     - Session is Initialized
     *  #UWBAPI_SESSION_DEINIT_SUCCESS   - Session is De-initialized
     *  #UWBAPI_SESSION_ACTIVATED        - Session is Busy
     *  #UWBAPI_SESSION_IDLE             - Session is Idle
     *  #UWBAPI_SESSION_ERROR            - Session Not Found
     *
     * if API returns not uwb::Status::SUCCESS, Session State is set to
     * UWBAPI_SESSION_ERROR
     */
    uwb::Status state(uint8_t& state);

    /**
     * @brief Start Ranging for a session. Before Invoking Start ranging its
     * mandatory to set all the ranging configurations.
     *
     * @return #uwb::Status::SUCCESS                 on success
     * @return #uwb::Status::NOT_INITIALIZED    if UWB stack is not initialized
     * @return #uwb::Status::SESSION_NOT_EXIST  if session is not initialized with
     *                                           sessionHandle
     * @return #uwb::Status::FAILED             otherwise
     * @return #uwb::Status::TIMEOUT            if command is timeout
     */
    uwb::Status start();
    /**
     * @brief stop the session
     *
     * @return uint8_t
     */
    uwb::Status stop();
    /**
     * @brief Set the Default App configs and Vendor configs
     *
     */
    void applyDefaults();

    
  
    bool channel(uint8_t channel);


    UWBRangingParams rangingParams;
    UWBAppParamList appParams;
    //UWBVendorParamList vendorParams;

protected:
    uint32_t sessID;
    
    uwb::SessionType type;
    bool isActive; // Indicates whether the session slot is in use
};

#endif // UWBSESSION_HPP
