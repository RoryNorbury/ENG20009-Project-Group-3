// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// Periodically read time and sensor values and store in SD card
#pragma once

#include "sensors.h"
#include "rtc.h"
#include "sd_card_logging"

namespace periodic_logging
{
    const int logPeriod = 30000; // in milliseconds
    int lastLogTime = 0;
}

void logSensordata()
{
    sensorData data;
    readSensors(&data);
    String time = rtc::getTimeString(true);
    logSensordata(data, time);
    Serial.println("Writing to sd card: [" + time + "]," + String(data.tempreature) + "," + String(data.humidity) + "," + String(data.pressure) + "," + String(data.gas_resistance) + "," + String(data.light));
}

void update_periodic_logging()
{
    if (millis() > periodic_logging::lastLogTime + periodic_logging::logPeriod)
    {
        periodic_logging::lastLogTime = millis();
        logSensordata();
    }
}