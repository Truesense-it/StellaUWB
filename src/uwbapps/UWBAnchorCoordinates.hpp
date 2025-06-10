// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBANCHORCOORDINATES_HPP
#define UWBANCHORCOORDINATES_HPP
#include "stdint.h"


/**
 * @brief utility class to be used for Downlink TDoA 
 * 
 */
class UWBAnchorCoordinates
{
public:
    uint8_t data[13]; // Array to hold bytes

public:
    UWBAnchorCoordinates()
    {
        memset(data, 0, sizeof(data));
    }

    void setCoordinatesAvailable(bool available)
    {
        data[0] = (data[0] & 0xFE) | available;
    }

    bool areCoordinatesAvailable() const
    {
        return data[0] & 0x01;
    }

    void setCoordinateSystem(bool wgs84)
    {
        data[0] = (data[0] & 0xFD) | (wgs84 << 1);
    }

    bool isWGS84() const
    {
        return data[0] & 0x02;
    }

    void setWGS84Coordinates(double latitude, double longitude, double altitude)
    {
        if (!isWGS84())
        {
            Serial.println("Invalid operation: Not in WGS-84 mode.");
            return;
        }

        uint64_t latBits = static_cast<uint64_t>((latitude + 90.0) * ((1LL << 33) - 1) / 180.0);
        uint64_t lonBits = static_cast<uint64_t>((longitude + 180.0) * ((1LL << 33) - 1) / 360.0);
        uint32_t altBits = static_cast<uint32_t>(altitude);

        // Pack latitude
        for (int i = 0; i < 5; i++)
        {
            data[1 + i] = (latBits >> (32 - i * 8)) & 0xFF;
        }

        // Pack longitude
        for (int i = 0; i < 5; i++)
        {
            data[5 + i] = (lonBits >> (32 - i * 8)) & 0xFF;
        }

        // Pack altitude
        for (int i = 0; i < 4; i++)
        {
            data[9 + i] = (i < 3) ? (altBits >> (24 - i * 8)) & 0xFF : data[9 + i] & 0xC0;
        }
    }

    void setRelativeCoordinates(int x, int y, int z)
    {
        if (isWGS84())
        {
            Serial.println("Invalid operation: Not in relative coordinates mode.");
            return;
        }

        uint32_t xBits = static_cast<uint32_t>(x);
        uint32_t yBits = static_cast<uint32_t>(y);
        uint32_t zBits = static_cast<uint32_t>(z);

        // Pack x coordinate
        for (int i = 0; i < 4; i++)
        {
            data[1 + i] = (xBits >> (24 - i * 8)) & 0xFF;
        }

        // Pack y coordinate
        for (int i = 0; i < 4; i++)
        {
            data[4 + i] = (yBits >> (24 - i * 8)) & 0xFF;
        }

        // Pack z coordinate
        for (int i = 0; i < 3; i++)
        {
            data[8 + i] = (zBits >> (16 - i * 8)) & 0xFF;
        }
    }
};


#endif/* UWBANCHORCOORDINATES */
