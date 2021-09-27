#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Syscalls/Memory.hpp>

void SysMMap(Registers *regs)
{
    if(regs->R8 == 1)
    {
        uint8_t *mem = (uint8_t*) FrameAllocator.RequestPageFrames(regs->R10);
        for(uint64_t i = 0; i < regs->R10; i++) 
            PagingManager.MapPage((uint8_t*) regs->RSI + i * 0x1000, mem + i * 0x1000);
    }
    else if(regs->R8 == 0)
        for(uint64_t i = 0; i < regs->R10; i++) 
            PagingManager.MapPage((uint8_t*) regs->RSI + i * 0x1000, FrameAllocator.RequestPageFrame());
}