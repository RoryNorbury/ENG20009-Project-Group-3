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
namespace sensor_alert_flags
{
    u_int8_t LIGHT = 1;
    u_int8_t GAS = 2;
    u_int8_t HUMIDITY = 4;
};
// @brief check if any sensor readings violate their thresholds
// @param lastData the last sensor readings
// @return true if any sensor readings violate their thresholds, false otherwise
u_int8_t check_alert_thresholds(sensorData lastData)
{
    u_int8_t output = 0;
    // TODO: implement exception if we implement handling them in a higher layer
    if (!sensors::inited)
    {
        // sensors are not initialised
        return 0;
    }
    if (lastData.light < sensor_thresholds::LIGHT)
    {
        output &= sensor_alert_flags::LIGHT;
    }
    if (lastData.gas_resistance > sensor_thresholds::GAS)
    {
        output &= sensor_alert_flags::GAS;
    }
    if (lastData.humidity > sensor_thresholds::HUMIDITY)
    {
        output &= sensor_alert_flags::HUMIDITY;
    }
    return output;
}