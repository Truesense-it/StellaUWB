// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBMACADDRESS_HPP
#define UWBMACADDRESS_HPP

/**
 * @brief class for managing UWB Mac addresses
 * 
 */
class UWBMacAddress {

public:
    enum Size { SHORT = 2, LONG = 8 };

private:
    uint8_t data[8];  // Max size covered
    Size currentSize;

public:
    /**
     * @brief Construct a new UWBMacAddress object, empty, only specifies the size
     * 
     * @param size 
     */
    UWBMacAddress(Size size = Size::SHORT) : currentSize(size) {
        memset(data, 0, sizeof(data));
    }

    /**
     * @brief same as above but with initialization data
     * 
     * @param size 
     * @param init_data 
     */
    UWBMacAddress(Size size, const uint8_t init_data[]) : currentSize(size) {
        if (init_data != nullptr) {
            memcpy(data, init_data, getSize());  // Use memcpy for efficiency
        } else {
            memset(data, 0, sizeof(data));  // Fallback to zero initialization
            //UWBHAL.Log_E("empty mac data");
        } 
    }

    

    // Copy constructor, assignment operator, set, get, etc. methods...

    void set(size_t index, uint8_t value) {
        if (index < getSize()) {
            data[index] = value;
        } else {
            //Serial.println(F("Index out of range"));  // Use Flash memory for strings
        }
    }

    uint8_t get(size_t index) const {
                if (index < getSize()) {
            return data[index];
        }
        //Serial.println(F("Index out of range"));
        return 0;  // Return zero if out of range
    }

    /**
     * @brief Get the length of Mac address
     * 
     * @return size_t, can be 2 or 8 respectively for short and extended addresses
     */
    size_t getSize() const {
        return currentSize == Size::SHORT ? 2 : 8;
    }

    /**
     * @brief Get a pointer to the array containing the actual Mac address
     * 
     * @return uint8_t* 
     */
    uint8_t *getData()
    {
        return (uint8_t *)&data[0];
    }
};


#endif //UWBMACADDRESS_HPP


