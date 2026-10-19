// AUTHOR: Rory Norbury || 105914122
// Functions to initialise and query the onboard rtc
#pragma once

#include <RTClib.h>

namespace rtc
{
    RTC_DS1307 rtc;

    /// @brief initialise rtc
    /// @return 0 on success, 1 on error
    bool initRTC(bool check_out_of_sync)
    {
        // initialise
        if (!rtc.begin())
        {
            Serial.println("Couldn't find RTC");
            return true;
        }
        // start rtc if it is not already running
        if (!rtc.isrunning())
        {
            Serial.println("RTC is not running, setting time");
            rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        }
        if (check_out_of_sync)
        {
            // Adjust rtc if it is too far from real time (>10 seconds)
            if (abs((rtc.now() - DateTime(F(__DATE__), F(__TIME__))).totalseconds()) > 10)
            {
                Serial.println("RTC is out of sync, setting time");
                rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            }
        }
            return false;
    }

    DateTime getDateTime()
    {
        return rtc.now();
    }

    String getTimeString(bool use_24_hour)
    {
        DateTime now = rtc.now();
        String out;
        if (use_24_hour)
        {
            char formatBuffer[] = "hh:mm:ss";
            out = String(now.toString(formatBuffer));
        }
        else
        {
            char formatBuffer[] = "hh:mm:ss ap";
            out = String(now.toString(formatBuffer));
        }
        return out;
    }
}