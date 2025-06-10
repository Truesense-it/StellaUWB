// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBMACADDRESSLIST_HPP
#define UWBMACADDRESSLIST_HPP

#include "UWBMacAddress.hpp"
#include <cstddef>


class UWBMacAddressList {
private:
    static const size_t MAX_SIZE = 12; // Maximum number of UWBMacAddress objects that can be stored
    UWBMacAddress arrays[MAX_SIZE];
    size_t count;
    UWBMacAddress::Size typeSize;

public:
    UWBMacAddressList(UWBMacAddress::Size size)
        : typeSize(size), count(0) {}

    void add(UWBMacAddress& array) {
        if (array.getSize() != typeSize) {
            //Serial.println(F("Array size mismatch"));
            return;
        }
        if (count < MAX_SIZE) {
            arrays[count++] = array;
        } else {
            //Serial.println(F("List is full, cannot add more items"));
        }
    }

    uint8_t* getAllData()  {
        static uint8_t result[MAX_SIZE * UWBMacAddress::LONG]; // Assuming a constant maximum address size
        uint8_t* ptr = result;
        for (size_t i = 0; i < count; ++i) {
            uint8_t* data = arrays[i].getData();
            for (size_t j = 0; j < arrays[i].getSize(); ++j) {
                *ptr++ = data[j];
            }
        }
        return result; // Note: this returns a pointer to a static array, so it should not and cannot be deleted.
    }

    uint32_t size()  {
        return count;
    }

    UWBMacAddress::Size macTypeSize() const {
        return typeSize;
    }
};

#endif /* UWBMACADDRESSLIST */
