#include <DateTime.hpp>
#include <IO/PIT.hpp>
#include <Syscalls/Timer.hpp>

#include <Display/Renderer.hpp>

void SysSleep(Registers *regs)
{
    int factor = 1;
    if(regs->RSI == 0) factor = 1000;
    PIT::Sleep(regs->RDI * factor);
}

void SysTime(Registers *regs)
{
    regs->RAX = (SystemDateTime.Second & 0x3F) | ((SystemDateTime.Minute & 0x3F) << 6) |
        ((SystemDateTime.Hour & 0x1F) << 12) | ((SystemDateTime.Weekday & 0x07) << 17) |
        ((SystemDateTime.Day & 0x1F) << 20) | ((SystemDateTime.Month & 0x0F) << 25) |
        ((SystemDateTime.Year & 0xFFFFFFFFl) << 29);
}