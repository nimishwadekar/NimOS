#pragma once

namespace RTC
{
    extern long long SecondsSinceBoot;

    void InitSystemTime(uint8_t datetime[8]);
    void Tick();
}