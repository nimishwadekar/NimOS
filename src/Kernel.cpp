#include <stdint.h>
#include "Display/Renderer.hpp"
#include "String.hpp"
#include "Logging.hpp"
#include "Memory/MemoryMap.hpp"
#include "Memory/PageFrameAllocator.hpp"
#include "Memory/Paging.hpp"
#include "Memory/PageTableManager.hpp"

// Kernel's main function.
void KernelStart(MemoryMap memoryMap)
{
    #ifdef LOGGING
    Logf("Kernel begin.\n");
    #endif

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

    FrameAllocator.Initialize(memoryMap);
    MainRenderer.Printf("Free memory = 0x%x\nUsed memory = 0x%x\nReserved memory = 0x%x\n\n", 
        FrameAllocator.FreeMemory, FrameAllocator.UsedMemory, FrameAllocator.ReservedMemory);

    PageTable *l4;
    asm volatile("mov %%cr3, %%rax" : "=a"(l4) : );
    MainRenderer.Printf("Level 4 table at 0x%x:\n", l4);
    
    MainRenderer.Printf("\n");
    uint64_t virt = 0b000000001'000000000'000000000'000000000'000000000000;
    MainRenderer.Printf("0x%x\n", virt);

    PageTableManager manager(l4);
    FrameAllocator.GetPage();
    FrameAllocator.GetPage();
    manager.MapPage((void*) virt, FrameAllocator.GetPage());

    for(int i = 0; i < 5; i++)
    {
        //PageTableEntry e = ((PageTable*) (l4->Entries[1].GetAddress()))->Entries[i];
        PageTableEntry e = l4->Entries[i];
        MainRenderer.Printf("Entry %u present = %u, address = 0x%x\n", i, e.GetFlag(PageTableFlags::Present), e.GetAddress());
    }

    uint32_t *n = (uint32_t*) (virt + 0x120);
    *n = 0xCAFEBABE;
    MainRenderer.Printf("\n%x\n", *(uint32_t*) 0x2120);

    manager.MapPage((void*) (0x8001000000), FrameAllocator.GetPage());

    while(true);
}