#include <DateTime.hpp>
#include <IO/RTC.hpp>

#include <Display/Renderer.hpp>
char buf[32];

namespace RTC
{
    long long SecondsSinceBoot = 0;

    void InitSystemTime(uint8_t datetime[8])
    {
        SystemDateTime.Initialise(datetime);
    }

    void Tick()
    {
        static bool half = false;

        if(!half) half = true;
        else
        {
            SecondsSinceBoot += 1;
            SystemDateTime.TickSecond();
            half = false;
        }
    }
}