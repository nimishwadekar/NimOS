#include <DateTime.hpp>
#include <IO/RTC.hpp>

namespace RTC
{
    long long SecondsSinceBoot = 0;

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