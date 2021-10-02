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
#include <Tasking/Process.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

// Kernel's main function.
void KernelStart(void)
{
    printf("Kernel initialized.\n\n");

    FILE *program = VFSOpenFile("usr/main.elf", "r");    
    uint8_t *programAddress = (uint8_t*) KernelHeap.Malloc(program->Length);
    if(VFSReadFile(program, programAddress, program->Length) != program->Length)
    {
        errorf("Program file read error.\n");
        while(true);
    }

    ELF::LoadInfo info = ELF::LoadELF(programAddress);
    void *programEntry = info.Entry;
    if(!programEntry)
    {
        errorf("Program file format incorrect.\n");
        while(true);
    }
    VFSCloseFile(program);
    KernelHeap.Free(programAddress);

    /********* TEMPORARY *****************/
    VFSOpenFile("someOuterFile", "r");
    VFSOpenFile("anotherDirectory/file", "r");
    /********* TEMPORARY *****************/

    MainRenderer.SetBackgroundColour(USER_COLOUR_BACK);
    MainRenderer.SetForegroundColour(USER_COLOUR_FRONT);
    MainRenderer.ClearScreen();

    if(PushProcess(programEntry, info.FirstAddress, info.PageCount) == -1)
    {
        errorf("Could not create root process.\n");
        while(true);
    }

    Process *root = PeekProcess();
    printf("stack: %x\nheap: %x\n", root->StackTop, root->HeapBase);
    JumpToUserMode((void*) &SyscallEntry, root->StackTop, root->HeapBase, programEntry); // Does not return here.

    errorf("CAME BACK TO THE KERNEL!!!\n");
    while(true);
}