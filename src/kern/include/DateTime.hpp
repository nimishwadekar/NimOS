#pragma once

#include <stdint.h>

class DateTime
{
    public:
    int Year;
    int Month;
    int Day;
    int Hour; // 24 hour format
    int Minute;
    int Second;

    int64_t SecondsSinceYear0;
    int TimeZoneOffsetHours;
    int TimeZoneOffsetMinutes;

    void Initialise(uint8_t datetime[8]);
    void TickSecond();

    struct Hour12
    {
        int Hour;
        bool PM;
    };

    // Minimum buffer length: 20 (24 hour format [ampm = 0]), 23 (12 hour format [ampm = 1]).
    char *ToString(char *buffer, int ampm);
    Hour12 Hour24ToHour12(int hour24);
};

extern DateTime SystemDateTime;