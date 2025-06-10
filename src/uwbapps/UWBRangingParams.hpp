// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBRANGINGPARAMS_HPP
#define UWBRANGINGPARAMS_HPP

#include "hal/uwb_types.hpp"
#include "hal/uwb_hal.hpp"
#include "UWBMacAddress.hpp"
#include "UWBMacAddressList.hpp"

// Constants preserved
#ifndef MAC_EXT_ADD_LEN
#define MAC_EXT_ADD_LEN 8
#endif

#ifndef MAX_NUM_RESPONDERS
#define MAX_NUM_RESPONDERS 12
#endif

class UWBRangingParams {
public:
    // Default constructor
    UWBRangingParams() {
        _ranging_params.device_role = uwb::DeviceRole::RESPONDER;
        _ranging_params.device_type = uwb::DeviceType::CONTROLEE;
        _ranging_params.multi_node_mode = uwb::MultiNodeMode::UNICAST;
        _ranging_params.mac_addr_mode = 0;
        _ranging_params.ranging_method = uwb::RangingMethod::SS_TWR;
        _ranging_params.scheduled_mode = uwb::ScheduledMode::CONTENTION;
        memset(_ranging_params.device_mac_addr, 0, MAC_EXT_ADD_LEN);
    }

    
    // Init from existing config
    UWBRangingParams(const uwb::RangingConfig& config) {
        _ranging_params.device_role = config.device_role;
        _ranging_params.device_type = config.device_type;
        _ranging_params.multi_node_mode = config.multi_node_mode;
        _ranging_params.mac_addr_mode = config.mac_addr_mode;
        _ranging_params.ranging_method = config.ranging_method;
        _ranging_params.scheduled_mode = config.scheduled_mode;
        memcpy(_ranging_params.device_mac_addr, config.device_mac_addr, MAC_EXT_ADD_LEN);
        memcpy(_ranging_params.dst_mac_addr, config.dst_mac_addr, sizeof(_ranging_params.dst_mac_addr));
        _ranging_params.no_of_controlees = config.no_of_controlees;
    }

    /**
     * @brief Set the Device Role 
     * 
     * In UWB (Ultra-Wideband) systems, device roles define how each device 
     * participates in ranging, communication, and network management. 
     * The roles are typically assigned based on the function each device serves
     * within a UWB session or network, which could involve initiating ranging, 
     * responding to distance measurements, or managing time synchronization 
     * across devices. Here are the main device roles in UWB:
     *
     * 1. Initiator
     *
     *    The initiator is the device that starts or "initiates" the ranging 
     *    process by sending out the first message (e.g., a "poll" message) in a
     *    Two-Way Ranging (TWR) or Time Difference of Arrival (TDoA) protocol.
     *    The initiator typically controls the timing and frequency of ranging 
     *    requests, which helps coordinate the process, especially in systems 
     *    where multiple devices are involved.
     *    Use Case: In secure access systems, a smartphone could act as the 
     *    initiator, sending a request to measure distance to an anchor (e.g., a
     *    car or door).
     *
     * 2. Responder
     *
     *    The responder is the device that responds to the initiator’s request 
     *    by replying to the initial poll message with its own response message,
     *    allowing the initiator to measure the time-of-flight (ToF) and 
     *    calculate distance.
     *    In protocols like Double-Sided Two-Way Ranging (DS-TWR), the responder
     *    plays a crucial role in sending back timestamps to enable accurate 
     *    ranging calculations.
     *    Use Case: A car in a keyless entry system might act as the responder, 
     *    replying to distance measurements initiated by a key fob or smartphone.
     *
     * 3. Anchor
     *
     *    Anchors are fixed devices with known locations in an environment. 
     *    In UWB systems, especially in positioning applications, anchors serve
     *    as reference points for mobile devices (tags) to calculate their 
     *    position.
     *    Anchors typically stay in listening mode, waiting for signals from 
     *    mobile devices, and they may work in configurations like Time 
     *    Difference of Arrival (TDoA) to determine a tag's location.
     *    
     *    A synchronization anchor in UWB systems is a specialized anchor that 
     *    is responsible for establishing and maintaining timing synchronization
     *    across multiple devices within a UWB network. Synchronization anchors 
     *    play a critical role in ensuring that UWB devices share a common time 
     *    reference, which is essential for accurate distance measurements, 
     *    efficient time-slot management, and coordinated communication.
     *    Over time, the clocks of individual devices may drift due to 
     *    environmental factors or slight manufacturing variances. The 
     *    synchronization anchor periodically broadcasts synchronization signals
     *    to correct these drifts.
     *    This ensures that all devices maintain synchronized timing, which is 
     *    necessary for applications requiring continuous, high-precision
     *     measurements, such as real-time location tracking.
     * 
     *    Use Case: In a warehouse with UWB-based real-time location systems 
     *    (RTLS), anchors are mounted at known points throughout the facility to
     *    help track the location of tagged assets.
     *
     * 4. Tag
     *
     *    A tag is a mobile device attached to an asset, person, or object whose
     *    position is being tracked within an environment. Tags communicate with
     *    anchors or other UWB infrastructure to determine their location.
     *    Tags usually have lower power requirements and may initiate the 
     *    ranging process with multiple anchors or operate in reverse TDoA 
     *    (rTDoA) setups, where they send out signals for anchors to measure.
     * 
     *    Use Case: In logistics or asset tracking, a tag might be attached to a
     *    pallet or tool, with its position tracked in real time by UWB anchors 
     *    around the facility.
     *
     * 5. Advertiser
     *    An advertiser in UWB is a device that periodically sends out broadcast messages to announce its presence, 
     *    allowing nearby devices to discover it and initiate communication or 
     *    ranging sessions if necessary. This role is fundamental for device 
     *    discovery, proximity-based services, and low-power operation in 
     *    various UWB applications, such as keyless entry, indoor positioning,
     *    and access control. 
     *
     *  6. Observer
     *    An observer in a UWB system is a device that passively listens to other devices' transmissions, gathering 
     *    information for location tracking, diagnostics, or security without 
     *    actively engaging in communication. This role is useful in 
     *    applications where low interference, data collection, or network 
     *    monitoring is necessary, providing valuable insights without impacting 
     *    the ongoing UWB communication.
     * 
     *  7. DL-TDoA Anchor
     *    A DL-TDoA (Downlink Time Difference of Arrival) anchor is a 
     *    specialized anchor used in Downlink Time Difference of Arrival 
     *    (DL-TDoA) positioning systems within UWB networks. 
     *    In DL-TDoA, anchors transmit timing signals, and mobile devices (such 
     *    as tags) measure the arrival times of these signals to calculate their
     *    own position. DL-TDoA is a form of one-way ranging where the mobile 
     *    device passively receives signals from multiple anchors, allowing it 
     *    to determine its location based on the difference in arrival times of 
     *    the signals.
     * 
     * 8. DL-TDoA Tag
     *    A DL-TDoA (Downlink Time Difference of Arrival) tag is a mobile device
     *    in a UWB positioning system that determines its own location by 
     *    passively receiving timing signals from multiple DL-TDoA anchors. 
     *    Unlike in traditional two-way ranging, where the tag actively 
     *    participates in message exchanges, a DL-TDoA tag operates in one-way 
     *    ranging mode, where it listens to signals from the anchors without 
     *    needing to transmit. 
     * 
     * kUWB_DeviceRole_Responder      = 0,
     * kUWB_DeviceRole_Initiator      = 1,
     * kUWB_DeviceRole_UT_Sync_Anchor = 2,
     * kUWB_DeviceRole_UT_Anchor      = 3,
     * kUWB_DeviceRole_UT_Tag         = 4,
     * kUWB_DeviceRole_Advertiser     = 5,
     * kUWB_DeviceRole_Observer       = 6,
     * kUWB_DeviceRole_DlTDoA_Anchor  = 7,
     * kUWB_DeviceRole_DlTDoA_Tag     = 8,
     * 
     * @param role 
     */
    void deviceRole(uwb::DeviceRole role) {
        _ranging_params.device_role = role;
    }

    /**
     * @brief Set the Device Type 
     * 
     * Device Type directly determines the operational function and behavior of 
     * a device within a UWB network. Each type—controller, controlee, observer,
     * or advertiser—has a well-defined role, ensuring that devices interact in 
     * a structured and efficient way according to the network’s design and the 
     * application’s requirements.
     * Device Types:
     *
     * Controller
     *     Function: 
     *     The controller is the primary device that manages the UWB 
     *     session or network, initiating and coordinating communication, 
     *     ranging sessions, or synchronization tasks.
     * 
     *     Responsibilities:
     *     Initiates ranging requests and orchestrates communication within 
     *     the network.
     *     Manages timing, coordinates sessions, and ensures that devices 
     *     operate in sync.
     *     Acts as the central point of control, making it essential for 
     *     coordinating multi-device interactions or handling access control 
     *     tasks.
     * 
     *     Use Case: In a keyless entry system, the smartphone often acts as the
     *     controller, initiating the secure ranging session with the car 
     *     (controlee).
     * 
     * Controlee
     *     Function: The controlee is a device that responds to the controller’s
     *     commands and participates in the UWB session based on the 
     *     instructions received.
     * 
     *     Responsibilities:
     *         Responds to ranging requests or other commands from the controller.
     *         Operates as a secondary device within the session, following the 
     *         controller’s timing and synchronization requirements.
     * 
     *     Use Case: In a keyless entry scenario, the car acts as the controlee,
     *     responding to ranging requests from the smartphone controller to 
     *     determine proximity.
     * 
     * Observer
     *     Function: The observer passively monitors UWB communications and 
     *     ranging exchanges between other devices without actively participating.
     * 
     *     Responsibilities:
     *     Listens to UWB signals to collect data, track positions, or gather 
     *     network diagnostics.
     *     Does not transmit or interfere with the ongoing ranging sessions but 
     *     gathers useful data for monitoring or analysis.
     * 
     *     Use Case: In an RTLS setup, observers can be used to monitor the 
     *     movement of tags or devices, gathering data without interacting with 
     *     each device.
     * 
     * Advertiser
     *     Function: The advertiser periodically broadcasts its presence to 
     *     nearby devices, allowing them to detect it and, if applicable, 
     *     initiate a session.
     * 
     *     Responsibilities:
     *     Sends out advertisement beacons or signals containing its identifier 
     *     or capabilities, enabling nearby devices to discover it.
     *     Often serves as a trigger for proximity-based actions or session 
     *     initiation by other devices (such as controllers).
     * 
     *     Use Case: In secure access applications, a door lock might act as an 
     *     advertiser, broadcasting its presence to nearby authorized devices, 
     *     such as a user’s smartphone, which then initiates a ranging session.
     * 
     * @param type  0x00: Controlee, 
     *              0x01: Controller, 
     *              0x02: Advertiser, 
     *              0x03: Observer
     */
    void deviceType(uwb::DeviceType type) {
        _ranging_params.device_type = type;
    }

    /**
     * @brief Set the Multi Node Mode 
     * 
     * Multi-Node Mode (often abbreviated as multi-node mode) is a feature that allows multiple UWB devices to participate in a single UWB session or network, coordinating their ranging and communication activities. This mode is particularly useful in applications requiring real-time location tracking or complex interactions among multiple devices, such as asset tracking, indoor navigation, and collaborative positioning.
     * Key Aspects of Multi-Node Mode in FiRa:
     *
     * Enables Multiple Devices in a Single Session:
     *    Multi-node mode allows several devices to communicate and perform 
     *    ranging in the same session, enabling multi-point ranging and 
     *    positioning.
     *    Devices in multi-node mode can perform ranging with each other either 
     *    directly or through coordinated communication, allowing for greater 
     *    flexibility in tracking or locating multiple assets or people.
     *
     * Coordinated Timing and Synchronization:
     *    To manage the increased complexity of multiple devices ranging at once,
     *    multi-node mode relies on strict timing coordination and 
     *    synchronization.
     *    A controller (designated by device type in FiRa) typically manages the
     *    session, coordinating when each device should range to avoid collisions
     *    and ensure timing accuracy.
     *    Devices in multi-node mode may use Time Division Multiple Access 
     *    (TDMA) or other scheduling mechanisms to ensure efficient 
     *    communication and prevent overlap.
     *
     * Improves Scalability for High-Density Applications:
     *    Multi-node mode is designed to support high-density environments where
     *    many UWB-enabled devices are present, such as warehouses, factories, 
     *    or event venues.
     *    By allowing multiple devices to range in a coordinated manner, 
     *    multi-node mode ensures scalability, making it suitable for 
     *    environments where numerous tags, anchors, and other UWB-enabled 
     *    devices need to coexist without interference.
     *
     * Enhanced Ranging Capabilities:
     *    In multi-node mode, devices can perform peer-to-peer ranging or 
     *    anchor-based ranging, depending on the setup.
     *    This mode allows for collaborative or distributed positioning, where 
     *    devices range with each other to determine relative distances or locations within a larger UWB network.
     *    Multi-node mode supports more complex ranging schemes, such as multi-anchor positioning (where a tag ranges with multiple anchors) or multi-tag tracking (where multiple tags are tracked simultaneously).
 *
     * Dynamic Role Allocation:
     *    Devices in multi-node mode can dynamically adjust roles (as controllers, controlees, observers, or advertisers) based on session requirements or specific tasks.
     *    This allows a device to act as a controller in one context and as a controlee or observer in another, depending on the session’s needs. This flexibility is essential for real-time adjustments in dynamic environments.
     * 
     * @param mode  0x00: Single device to Single device (Unicast),
     *              0x01: One to Many,
     *              0x02: Many to Many,
     */
    void multiNodeMode(uwb::MultiNodeMode mode) {
        _ranging_params.multi_node_mode = mode;
    }

    /**
     * @brief Set the Mac Address Mode 
     * 
     * Devices can use either 2-byte or 8-byte MAC addresses as defined by the 
     * IEEE 802.15.4z standard, with each mode providing different levels of 
     * address uniqueness, scalability, and efficiency. These address lengths 
     * allow UWB devices to identify each other within a network, either in 
     * compact or extended form.
     * 
     * 2-byte mode is ideal for simple, localized UWB networks where minimal 
     * overhead and power efficiency are prioritized, with a limited number of 
     * devices.
     * 
     * 8-byte mode is preferred in large-scale or global UWB applications where 
     * devices need unique identification across multiple networks or geographic
     * areas.
     * 
     * @param mode  0: 2 bytes
     *              2: 8 bytes
     */
    void macAddrMode(uint8_t mode) {
        _ranging_params.mac_addr_mode = mode;
    }

    /**
     * @brief Set the Device Mac Address 
     * 
     * @param addr 
     */
    void deviceMacAddr(const uint8_t addr[], size_t len) {
        if (len <= MAC_EXT_ADD_LEN) {
            memcpy(_ranging_params.device_mac_addr, addr, len);
        }
        _ranging_params.mac_addr_mode = len == 2 ? 0 : 2;
    }
    
    void deviceMacAddr(UWBMacAddress addr) {
        if (addr.getSize() == 2) {
            memcpy(_ranging_params.device_mac_addr,  addr.getData(), 2);
            _ranging_params.mac_addr_mode = 0;
        } else {
            memcpy(_ranging_params.device_mac_addr, addr.getData(), 8);
            _ranging_params.mac_addr_mode = 2;
        }
    }

    void rangingRoundUsage(uwb::RangingMethod rangingRound) {
        _ranging_params.ranging_method = rangingRound;
    }

    void scheduledMode(uwb::ScheduledMode mode) {
        _ranging_params.scheduled_mode = mode;
    }
    void destinationMacAddr(UWBMacAddressList& dstMacAddrList) {
        memcpy(_ranging_params.dst_mac_addr, dstMacAddrList.getAllData(), 
               dstMacAddrList.size() * MAC_EXT_ADD_LEN);
    }
    void destinationMacAddr(UWBMacAddress& dstMacAddr) {
        if (dstMacAddr.getSize() == 2) {
            memcpy(_ranging_params.dst_mac_addr, dstMacAddr.getData(), 2);
        } else {
            memcpy(_ranging_params.dst_mac_addr, dstMacAddr.getData(), 8);
        }
    }
    void noOfControlees(uint8_t no_of_controlees) {
        if (no_of_controlees <= MAX_NUM_RESPONDERS) {
            _ranging_params.no_of_controlees = no_of_controlees;
        } else {
            // Handle error: too many controlees
            // UWBHAL.Log_E("Too many controlees specified");
        }
    }

    // Getter methods
    uwb::DeviceRole deviceRole() {
        return _ranging_params.device_role;
    }

    uwb::DeviceType deviceType() {
        return _ranging_params.device_type;
    }

    uwb::MultiNodeMode multiNodeMode() {
        return _ranging_params.multi_node_mode;
    }

    uint8_t macAddrMode() {
        return _ranging_params.mac_addr_mode;
    }

    const uint8_t* deviceMacAddr() {
        return _ranging_params.device_mac_addr;
    }

    uwb::RangingMethod rangingRoundUsage() {
        return _ranging_params.ranging_method;
    }

    uwb::ScheduledMode scheduledMode() {
        return _ranging_params.scheduled_mode;
    }
    uint8_t noOfControlees() {
        return _ranging_params.no_of_controlees;
    }
    const uint8_t* destinationMacAddr() {
        return _ranging_params.dst_mac_addr;
    }

private:
    uwb::RangingConfig _ranging_params;
    
};

#endif /* UWBRANGINGPARAMS_HPP */
