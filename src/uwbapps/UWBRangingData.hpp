// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBRANGINGDATA_HPP
#define UWBRANGINGDATA_HPP

#include <stdint.h>
#include "hal/uwb_types.hpp"

// // Define measurement type arrays using HAL types
// typedef uwb::twr_mesr TwrMeasurement_[uwb::MAX_RESPONDERS];
// //#define RangingMeasures TwrMeasurement_&
// //#define RangingMeasures const uwb::twr_mesr (&)[uwb::MAX_RESPONDERS];
// using RangingMeasures = const uwb::twr_mesr (&)[uwb::MAX_RESPONDERS];

using RangingMeasures = uwb::twr_mesr*;
//typedef uwb::tdoa_mesr TdoaMeasurement_[uwb::MAX_TDOA_MEASURES];
//#define RangingMesrTdoas TdoaMeasurement_&
using RangingMesrTdoas = uwb::tdoa_mesr*;
typedef uwb::dltdoa_mesr DlTdoaMeasurement_[uwb::MAX_TDOA_MEASURES];
#define RangingMesrDlTdoas DlTdoaMeasurement_&

typedef uwb::owr_aoa_mesr OwrAoaMeasurement_[uwb::MAX_OWR_AOA_MEASURES];
#define RangingMesrOwrAoas OwrAoaMeasurement_&

class UWBRangingData {
public:
    // Default constructor
    UWBRangingData();

    // Constructor from RangingResult
    UWBRangingData(const uwb::RangingResult& result);

    /**
    * @brief API to get the rcr indication
    * the Received Confirmation Response (RCR) is a signal 
    * used to confirm the successful reception of a message 
    * in a UWB ranging sequence
    */
    uint8_t rcrIndication() const;

    /**  
    * @brief get the measurement type
    * it can be one of:
    * MeasurementType::ONE_WAY       = 0x00
    * MeasurementType::TWO_WAY       = 0x01
    * MeasurementType::DL_TDOA       = 0x02
    * MeasurementType::OWR_AOA       = 0x03
    */
    uint8_t measureType() const;

    /**  
    * @brief get the MAC address mode
    * it can be one of:
    * MacAddressMode::SHORT               = 0x00
    * MacAddressMode::EXTENDED           = 0x01
    * MacAddressMode::EXTENDED_AND_HEADER = 0x02
    */
    uint8_t macMode() const;

    /**  
    * @brief return the number of measurements in this notification
    */
    uint8_t available() const;

    /** 
    * @brief Each message in a UWB session is assigned a unique sequence number. 
    * This number increases incrementally with each message, allowing devices
    * to track the order of messages sent and received.
    * It helps detect missing or out-of-order messages, which can lead to 
    * inaccuracies in the ranging process.
    */
    uint32_t seqCtr() const;

    /** 
    * @brief Get the session Handle for the current notification
    */
    uint32_t sessionHandle() const;

    /** 
    * @brief Get the current ranging interval, in milliseconds
    */
    uint32_t currRangeInterval() const;

    /**
     * @brief return the data structure for Two-Way-Ranging measurement
     */
    const RangingMeasures twoWayRangingMeasure() const;

    /** 
    * @brief return the data structure for a TDoA measurement
    */
    //const RangingMesrTdoas tdoaMeasure() const;

    /**  
    * @brief return the data structure for a Downlink TDoA measurement
    */
    //const RangingMesrDlTdoas dlTdoaMeasure() const;



private:
    
    uwb::RangingResult result;
    
};

#endif // UWBRANGINGDATA_HPP
