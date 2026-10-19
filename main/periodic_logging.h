// AUTHOR: Vakshika Kamalraj || Student ID: 104852959
// Periodically read time and sensor values and store in SD card
#pragma once

#include "sensors.h"
#include "rtc.h"

namespace periodoc_logging
{
    const int logPeriod = 30000; // in milliseconds
    int lastLogTime = 0;
}
    
// TEMPORARY UNTIL LOGGING FUNCTIONS ARE ADDED
void logSensordata(sensorData data, DateTime time) {}

void update_periodic_logging()
{
    if (millis() > periodoc_logging::lastLogTime + periodoc_logging::logPeriod)
    {
        periodoc_logging::lastLogTime = millis();
        sensorData data;
        readSensors(&data);
        DateTime time = rtc::getDateTime();
        logSensordata(data, time);
    }
}