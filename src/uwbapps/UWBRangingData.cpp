// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "UWBRangingData.hpp"

// Default constructor
UWBRangingData::UWBRangingData() : result{} {
    // Initialize ranging result
    result.rcr_indication = 0;
    result.ranging_measure_type = (uint8_t) uwb::MeasurementType::ONE_WAY;
    result.mac_addr_mode_indicator = (uint8_t) uwb::MacAddressMode::SHORT;
    result.no_of_measurements = 0;
    result.sequence_number = 0;
    result.session_handle = 0;
    result.range_interval_ms = 0;

    // Initialize all measurement types in union
    memset(&result.measurements, 0, sizeof(uwb::RangingMeasurements));
   
}

// Constructor from RangingResult
UWBRangingData::UWBRangingData(const uwb::RangingResult& input_result)  {
    // Copy measurements based on measurement type
    result = input_result;
    //result.measurements = input_result.measurements;
}

uint8_t UWBRangingData::rcrIndication() const {
    return result.rcr_indication;
}

uint8_t UWBRangingData::measureType() const {
    return result.ranging_measure_type;
}

uint8_t UWBRangingData::macMode() const {
    return result.mac_addr_mode_indicator;
}

uint8_t UWBRangingData::available() const {
    return result.no_of_measurements;
}

uint32_t UWBRangingData::seqCtr() const {
    return result.sequence_number;
}

uint32_t UWBRangingData::sessionHandle() const {
    return result.session_handle;
}

uint32_t UWBRangingData::currRangeInterval() const {
    return result.range_interval_ms;
}

const RangingMeasures UWBRangingData::twoWayRangingMeasure() const {
    return (const RangingMeasures) result.measurements.twr;
}

// const RangingMesrTdoas UWBRangingData::tdoaMeasure() const {
//     return (const RangingMesrTdoas) result.measurements.tdoa;
// }
// const RangingMesrDlTdoas UWBRangingData::dlTdoaMeasure() const {
//     return result.measurements.dltdoa;
// }





