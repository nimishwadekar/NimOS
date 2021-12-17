#pragma once

#include <stdint.h>

class DateTime
{
    public:
    int Year;
    int Month; // 1 - 12.
    int Day; // 1 - (28/29/30/31).
    int Weekday; // 1 (Sun) - 7 (Sat).
    int Hour; // 0 - 23.
    int Minute; // 0 - 59.
    int Second; // 0 - 59.

    int TimeZoneOffsetHours;
    int TimeZoneOffsetMinutes;

    /*
     * Values in BCD.
     * 0 - century (19/20), 1 - year, 2 - month, 3 - day, 4 - weekday, 5 - hour, 6 - minute, 7 - second.
    */
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

    // Minimum buffer length: 30.
    char *ToString(char *buffer, int ampm);
    Hour12 Hour24ToHour12(int hour24);
};

extern DateTime SystemDateTime;