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

    int TimeZoneOffsetHours;
    int TimeZoneOffsetMinutes;

    void Initialise(uint8_t datetime[8]);

    void TickYear();
    void TickMonth();
    void TickDay();
    void TickHour();
    void TickMinute();
    void TickSecond();

    void TickYearBack();
    void TickMonthBack();
    void TickDayBack();
    void TickHourBack();
    void TickMinuteBack();
    void TickSecondBack();

    bool IsYearLeap(int year);

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