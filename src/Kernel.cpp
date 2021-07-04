#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Memory/MemoryMap.hpp"
#include "Memory/PageFrameAllocator.hpp"

// Kernel's main function.
void KernelStart(MemoryMap memoryMap)
{
    MainRenderer.ClearScreen();
    MainRenderer.Printf("Started Kernel.\n\n");

    MainRenderer.Printf("Memory map:\nTotal memory = %u KB\n", memoryMap.MemorySizeKB);
    for(unsigned int i = 0; i < memoryMap.EntryCount; i++)
    {
        MainRenderer.Printf("Begin = 0x%x    End = 0x%x    Size = 0x%x    Type = %s\n", memoryMap.Entries[i].Address, 
            memoryMap.Entries[i].Address + MemoryMapEntry_Size(memoryMap.Entries[i]) - 1, 
            MemoryMapEntry_Size(memoryMap.Entries[i]), MemoryMapEntry_TypeString(memoryMap.Entries[i]));
    }
    MainRenderer.Printf("\n");

    PageFrameAllocator pageFrameAllocator;
    pageFrameAllocator.Initialize(memoryMap);
    MainRenderer.Printf("Free memory = 0x%x\nUsed memory = 0x%x\nReserved memory = 0x%x\n\n", 
        pageFrameAllocator.FreeMemory, pageFrameAllocator.UsedMemory, pageFrameAllocator.ReservedMemory);

    MainRenderer.Printf("First page frame obtained = 0x%x\n", pageFrameAllocator.GetPage());
    for(long i = 0; i < 158; i++) pageFrameAllocator.GetPage();
    void *a, *b;
    MainRenderer.Printf("Page frame obtained = 0x%x\n", a = pageFrameAllocator.GetPage());
    MainRenderer.Printf("Page frame obtained = 0x%x\n", b = pageFrameAllocator.GetPage());
    pageFrameAllocator.FreePage(a);
    pageFrameAllocator.FreePage(b);
    MainRenderer.Printf("Page frame obtained = 0x%x\n", a = pageFrameAllocator.GetPage());
    MainRenderer.Printf("Page frame obtained = 0x%x\n", a = pageFrameAllocator.GetPage());

    while(true);
}