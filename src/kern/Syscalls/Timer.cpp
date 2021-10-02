#include <IO/PIT.hpp>
#include <Syscalls/Timer.hpp>

void SysSleep(Registers *regs)
{
    int factor = 1;
    if(regs->RSI == 0) factor = 1000;
    PIT::Sleep(regs->RDI * factor);
}