// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#include "NearbyBlePeerNotify.hpp"

#include "local/BLELocalCharacteristic.h"
#include "utility/ATT.h"
#include "utility/HCI.h"

#ifndef ATT_OP_HANDLE_NOTIFY
#define ATT_OP_HANDLE_NOTIFY 0x1B
#endif

class NearbyBleCharacteristicAccessor : public BLECharacteristic {
public:
    static BLELocalCharacteristic *localOf(BLECharacteristic &characteristic)
    {
        return static_cast<NearbyBleCharacteristicAccessor &>(characteristic).local();
    }
};

class NearbyBleLocalCharacteristicAccessor : public BLELocalCharacteristic {
public:
    static uint16_t valueHandleOf(BLELocalCharacteristic *local)
    {
        return static_cast<NearbyBleLocalCharacteristicAccessor *>(local)->valueHandle();
    }
};

// ArduinoBLE stores addresses as _address[0..5] but String address() prints
// them reversed: _address[5] first in the string (see BLEDevice.cpp).
static bool parseBleMac(const String &address, uint8_t mac[6])
{
    if (address.length() < 17) {
        return false;
    }

    for (int i = 0; i < 6; i++) {
        int pos = (5 - i) * 3;
        char hex[3] = {address[pos], address[pos + 1], 0};
        mac[i] = (uint8_t)strtoul(hex, NULL, 16);
    }
    return true;
}

uint16_t nearbyBlePeerConnHandle(BLEDevice peer)
{
    uint8_t mac[6];
    if (!parseBleMac(peer.address(), mac)) {
        return 0xffff;
    }

    uint16_t connHandle = ATT.connectionHandle(0, mac);
    if (connHandle == 0xffff) {
        connHandle = ATT.connectionHandle(1, mac);
    }
    return connHandle;
}

uint16_t nearbyBleValueHandle(BLECharacteristic &characteristic)
{
    BLELocalCharacteristic *local = NearbyBleCharacteristicAccessor::localOf(characteristic);
    if (local == nullptr) {
        return 0;
    }
    return NearbyBleLocalCharacteristicAccessor::valueHandleOf(local);
}

bool nearbyBleNotifyPeer(BLEDevice peer, uint16_t valueHandle, const uint8_t *data, int length)
{
    if (valueHandle == 0 || data == nullptr || length <= 0) {
        return false;
    }

    uint16_t connHandle = nearbyBlePeerConnHandle(peer);
    if (connHandle == 0xffff) {
        return false;
    }

    uint16_t mtu = ATT.mtu(connHandle);
    if (mtu < 4) {
        mtu = 23;
    }

    int maxPayload = (int)mtu - 3;
    if (length > maxPayload) {
        return false;
    }

    uint8_t pdu[mtu];
    pdu[0] = ATT_OP_HANDLE_NOTIFY;
    pdu[1] = valueHandle & 0xff;
    pdu[2] = (valueHandle >> 8) & 0xff;
    memcpy(&pdu[3], data, length);

    return HCI.sendAclPkt(connHandle, ATT_CID, 3 + length, pdu) == 0;
}
