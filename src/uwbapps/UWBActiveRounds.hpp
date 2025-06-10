// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBACTIVEROUNDS
#define UWBACTIVEROUNDS

#include "UWB.hpp"

class UWBActiveRounds {
private:
    phActiveRoundsConfig_t *configs;
    size_t size;
    size_t capacity;  // This is the maximum number of elements the array can hold

public:
    // Constructor with predefined capacity
    UWBActiveRounds(size_t capacity) : size(0), capacity(capacity) {
        configs = new phActiveRoundsConfig_t[capacity];
        memset(configs, 0, sizeof(phActiveRoundsConfig_t) * capacity);
    }

    // Destructor
    ~UWBActiveRounds() {
        for (size_t i = 0; i < size; ++i) {
            delete[] configs[i].responderMacAddressList;
            delete[] configs[i].responderSlots;
        }
        delete[] configs;
    }

    // Add a new configuration to the array
    void addConfig(const phActiveRoundsConfig_t& config) {
        if (size >= capacity) {
            // Capacity limit reached, cannot add more
            return;
        }
        // Deep copy the provided config
        configs[size] = config;
        if (config.responderMacAddressList) {
            configs[size].responderMacAddressList = new uint8_t[config.noofResponders];
            memcpy(configs[size].responderMacAddressList, config.responderMacAddressList, config.noofResponders);
        }
        if (config.responderSlots) {
            configs[size].responderSlots = new uint8_t[config.noofResponders];
            memcpy(configs[size].responderSlots, config.responderSlots, config.noofResponders);
        }
        size++;
    }

    // Access a config element
    phActiveRoundsConfig_t& get(size_t index) {
        if (index < size) {
            return configs[index];
        }
        // Return the first element if index is out of bounds (unsafe alternative to exceptions)
        return configs[0];
    }

    // Get current size of the array
    size_t getSize() const {
        return size;
    }

    phActiveRoundsConfig_t * getConfigs()
    {
        return configs;
    }
};


#endif/* UWBACTIVEROUNDS */
