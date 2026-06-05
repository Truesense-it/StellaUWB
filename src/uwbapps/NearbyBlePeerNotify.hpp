// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef NEARBY_BLE_PEER_NOTIFY_HPP
#define NEARBY_BLE_PEER_NOTIFY_HPP

#include <ArduinoBLE.h>

/**
 * @brief Helpers for per-peer BLE notify in multi-session Nearby Interaction.
 *
 * ArduinoBLE writeValue() on a notify characteristic broadcasts to all
 * connected centrals. These helpers send ATT notify PDUs to one peer only.
 */

uint16_t nearbyBleValueHandle(BLECharacteristic &characteristic);
uint16_t nearbyBlePeerConnHandle(BLEDevice peer);
bool nearbyBleNotifyPeer(BLEDevice peer, uint16_t valueHandle, const uint8_t *data, int length);

#endif
