#pragma once

#include <stdint.h>

namespace PIT
{
    extern volatile double SecondsSinceBoot;
    const uint64_t BaseFrequency = 1193182;

    void Sleep(const uint64_t milliseconds);

    void SetDivisor(uint16_t divisor);
    uint64_t GetFrequency(void);
    void SetFrequency(const uint64_t frequency);
    void Tick(void);
}