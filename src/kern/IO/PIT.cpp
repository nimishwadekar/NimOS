#include <IO/PIT.hpp>
#include <IO/Port.hpp>

namespace PIT
{
    volatile double SecondsSinceBoot = 0;
    uint16_t Divisor = 65535;

    void Sleep(const uint64_t milliseconds)
    {
        double startTime = SecondsSinceBoot * 1000;
        while(SecondsSinceBoot * 1000 < startTime + milliseconds) asm volatile("hlt");
    }

    void SetDivisor(uint16_t divisor)
    {
        if(divisor < 100) divisor = 100;
        Divisor = divisor;
        outb(0x40, (uint8_t) (divisor & 0x00FF));
        io_wait();
        outb(0x40, (uint8_t) ((divisor & 0xFF00) >> 8));
    }

    inline uint64_t GetFrequency(void)
    {
        return BaseFrequency / Divisor;
    }

    void SetFrequency(const uint64_t frequency)
    {
        SetDivisor(BaseFrequency / frequency);
    }

    void Tick(void)
    {
        SecondsSinceBoot += 1.0 / GetFrequency();
    }
}