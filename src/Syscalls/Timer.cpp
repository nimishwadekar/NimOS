#include <IO/PIT.hpp>
#include <Syscalls/Timer.hpp>

void SysSleep(Registers *regs)
{
    int factor = 1;
    if(regs->RDX == 0) factor = 1000;
    PIT::Sleep(regs->R10 * factor);
}