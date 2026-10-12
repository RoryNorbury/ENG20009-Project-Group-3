// AUTHOR: RORY NORBURY | 105914122
// Function for threshold alert check
#pragma once

#include "sensors.h"

namespace sensor_thresholds
{
    float LIGHT = 300;
    float GAS = 10;// I have no idea what a normal gas reading is
    float HUMIDITY = 70;
}

// @brief check if any sensor readings violate their thresholds
// @param lastData the last sensor readings
// @return true if any sensor readings violate their thresholds, false otherwise
bool over_thresholds(sensorData lastData)
{
    // TODO: implement exception if we implement handling them in a higher layer
    if (!sensors::inited)
    {
        // sensors are not initialised
        return false;
    }
    if (lastData.light < sensor_thresholds::LIGHT
        || lastData.gas_resistance > sensor_thresholds::GAS
        || lastData.humidity > sensor_thresholds::HUMIDITY )
    {
        return true;
    }
    return false;
}