#include <stdint.h>
#include <Display/Renderer.hpp>
#include <IO/Keyboard.hpp>
#include <Syscalls/StdIO.hpp>

void SysPrintN(Registers *regs)
{
    char *str = (char*) regs->RDI;
    for(uint64_t n = regs->RSI; n > 0; str++, n--)
    {
        MainRenderer.PutChar(*str);
    }
    regs->RAX = regs->RSI;
}


void SysPrint(Registers *regs)
{
    char *str = (char*) regs->RDI;
    uint64_t n = 0;
    for( ; *str != 0; str++, n++)
    {
        MainRenderer.PutChar(*str);
    }
    regs->RAX = n;
}


void SysInputN(Registers *regs)
{
    uint64_t n = regs->RSI;
    char *dst = (char*) regs->RDI;
    for( ; n > 0; n--, dst++)
    {
        while(KBBuffer.IsEmpty());

        *dst = KBBuffer.Dequeue();
        MainRenderer.PutChar(*dst);
    }
    regs->RAX = n;
}


void SysInput(Registers *regs)
{
    char *dst = (char*) regs->RDI;
    uint64_t n = 0;
    do
    {
        while(KBBuffer.IsEmpty());

        *dst = KBBuffer.Dequeue();
        MainRenderer.PutChar(*dst);
        n += 1;
    } while(*(dst++) != '\n');
    regs->RAX = n;
}