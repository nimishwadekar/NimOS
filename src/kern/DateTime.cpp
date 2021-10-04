#include <DateTime.hpp>
#include <Environment.hpp>
#include <String.hpp>

DateTime SystemDateTime;

#include <Display/Renderer.hpp>

void DateTime::Initialise(uint8_t datetime[8])
{
    Year = (datetime[0] >> 4) * 1000 + (datetime[0] & 0x0F) * 100 +
        (datetime[1] >> 4) * 10 + (datetime[1] & 0x0F);
    Month = (datetime[2] >> 4) * 10 + (datetime[2] & 0x0F);
    Day = (datetime[3] >> 4) * 10 + (datetime[3] & 0x0F);
    Hour = (datetime[4] >> 4) * 10 + (datetime[4] & 0x0F);
    Minute = (datetime[5] >> 4) * 10 + (datetime[5] & 0x0F);
    Second = (datetime[6] >> 4) * 10 + (datetime[6] & 0x0F);

    unsigned char *zone = (unsigned char*) Environment.Timezone;
    if(zone)
    {
        bool forward = true;
        if(*zone == '+') zone++;
        else if(*zone == '-')
        {
            forward = false;
            zone++;
        }

        TimeZoneOffsetHours = atoi(strtok(zone, ":"));
        TimeZoneOffsetMinutes = atoi(strtok(nullptr, ""));
        if(TimeZoneOffsetHours == 0 && TimeZoneOffsetMinutes == 0) return;

        if(forward)
        {
            for(int i = 0; i < TimeZoneOffsetMinutes; i++) TickMinute();
            for(int i = 0; i < TimeZoneOffsetHours; i++) TickHour();
        }
        else
        {
            for(int i = 0; i < TimeZoneOffsetMinutes; i++) TickMinuteBack();
            for(int i = 0; i < TimeZoneOffsetHours; i++) TickHourBack();
        }
    }
    else TimeZoneOffsetHours = TimeZoneOffsetMinutes = 0;
}


static int daysPerMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void DateTime::TickYear()
{
    Year++;
}

void DateTime::TickMonth()
{
    Month++;
    if(Month > 12)
    {
        Month = 1;
        TickYear();
    }
}

void DateTime::TickDay()
{
    Day++;
    if(Day < 29) return;

    int days = (Month == 2 && IsYearLeap(Year)) ? 29 : daysPerMonth[Month];
    if(Day > days)
    {
        Day = 1;
        TickMonth();
    }
}

void DateTime::TickHour()
{
    Hour++;
    if(Hour > 23)
    {
        Hour = 0;
        TickDay();
    }
}

void DateTime::TickMinute()
{
    Minute++;
    if(Minute > 59)
    {
        Minute = 0;
        TickHour();
    }
}

void DateTime::TickSecond()
{
    Second++;
    if(Second > 59)
    {
        Second = 0;
        TickMinute();
    }
}

void DateTime::TickYearBack()
{
    Year--;
}

void DateTime::TickMonthBack()
{
    Month--;
    if(Month < 1)
    {
        Month = 12;
        TickYearBack();
    }
}

void DateTime::TickDayBack()
{
    Day--;
    if(Day > 0) return;

    TickMonthBack();
    Day = (Month == 2 && IsYearLeap(Year)) ? 29 : daysPerMonth[Month];
}

void DateTime::TickHourBack()
{
    Hour--;
    if(Hour < 0)
    {
        Hour = 23;
        TickDayBack();
    }
}

void DateTime::TickMinuteBack()
{
    Minute--;
    if(Minute < 0)
    {
        Minute = 59;
        TickHourBack();
    }
}

void DateTime::TickSecondBack()
{
    Second--;
    if(Second < 0)
    {
        Second = 59;
        TickMinuteBack();
    }
}


bool DateTime::IsYearLeap(int year)
{
    if(year % 400 == 0) return true;
    if(year % 100 == 0) return false;
    return year % 4 == 0;
}


DateTime::Hour12 DateTime::Hour24ToHour12(int hour24)
{
    Hour12 hr;
    if(hour24 < 12) hr.PM = false;
    else
    {
        hour24 -= 12;
        hr.PM = true;
    }

    if(hour24 == 0) hour24 = 12;
    hr.Hour = hour24;
    return hr;
}


char *DateTime::ToString(char *buffer, int ampm)
{
    char *buf = buffer;
    Hour12 hr = Hour24ToHour12(Hour);

    buffer = ultona(Year, 10, 4, buffer) + 4;
    *(buffer++) = '-';
    buffer = ultona(Month, 10, 2, buffer) + 2;
    *(buffer++) = '-';
    buffer = ultona(Day, 10, 2, buffer) + 2;
    *(buffer++) = ' ';
    buffer = ultona((ampm ? hr.Hour : Hour), 10, 2, buffer) + 2;
    *(buffer++) = ':';
    buffer = ultona(Minute, 10, 2, buffer) + 2;
    *(buffer++) = ':';
    buffer = ultona(Second, 10, 2, buffer) + 2;
    if(!ampm) return buf;

    *(buffer++) = ' ';
    strcpy((hr.PM ? "PM" : "AM"), buffer);
    return buf;
}