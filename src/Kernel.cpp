#include <stdint.h>
#include <Display/Renderer.hpp>
#include <FS/VFS.hpp>
#include <IO/Keyboard.hpp>
#include <IO/PIT.hpp>
#include <IO/Port.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <String.hpp>
#include <Syscalls/Syscall.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

extern "C" void BeginUserMode(uint32_t *fb);

// Kernel's main function.
void KernelStart(void)
{
    printf("Kernel initialized.\n\n");

    void *userStack = FrameAllocator.RequestPageFrame();
    PagingManager.MapPage((void*) USER_STACK_TOP, userStack);

    FILE program = VFSOpenFile("usr/test.elf", "r");
    int64_t pages = (program.Length / 0x1000) + 1;
    uint8_t *programAddress = (uint8_t*) FrameAllocator.RequestPageFrames(pages);
    for(int64_t i = 0; i < pages; i++) PagingManager.MapPage(programAddress + i * 0x1000, programAddress + i * 0x1000);
    VFSReadFile(&program, programAddress, program.Length);

    void *programEntry = ELF::LoadELF(programAddress);
    VFSCloseFile(&program);

    MainRenderer.SetBackgroundColour(USER_COLOUR_BACK);
    MainRenderer.SetForegroundColour(USER_COLOUR_FRONT);
    MainRenderer.ClearScreen();
    
    JumpToUserMode((void*) &SyscallEntry, (uint8_t*) USER_STACK_TOP + 0x1000, programEntry); // Does not return here.

    errorf("CAME BACK TO THE KERNEL!!!\n");

    /* while(true)
    {
        asm volatile("hlt");
        while(!KBBuffer.IsEmpty())
        {
            char c = KBBuffer.Dequeue();
            printf("%c", c);
            if(c == 'q')
            {
                outw(0x604, 0x2000);
            }
        }
    } */
}