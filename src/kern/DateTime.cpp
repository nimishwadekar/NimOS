#include <DateTime.hpp>
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
}


void DateTime::TickSecond()
{

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