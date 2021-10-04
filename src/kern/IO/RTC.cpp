#include <DateTime.hpp>
#include <IO/RTC.hpp>

#include <Display/Renderer.hpp>
char buf[32];

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
            printf("%s\n", SystemDateTime.ToString(buf, 1));
        }
    }
}