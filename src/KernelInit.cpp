#include <stdint.h>
#include "bootboot.h"
#include "Fonts/PSF.hpp"
#include "Display/Framebuffer.hpp"
#include "Display/Renderer.hpp"
#include "Memory/MemoryMap.hpp"
#include "IO/Serial.hpp"
#include "Logging.hpp"
#include "GDT.hpp"
#include "Memory/PageFrameAllocator.hpp"
#include "Memory/Paging.hpp"
#include "Memory/PageTableManager.hpp"
#include "Interrupts/Interrupts.hpp"
#include "Memory/Heap.hpp"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

extern volatile unsigned char _binary_font_psf_start;

extern void KernelStart(void);

#define HEAP_ADDRESS 0xFFFFFFFF00000000

// Entry point into kernel, called by Bootloader.
void main()
{
    Framebuffer framebuffer((uint32_t*) &fb, (Framebuffer::FBType) bootboot.fb_type, 
        bootboot.fb_size, bootboot.fb_width, bootboot.fb_height, bootboot.fb_scanline / 4);
    PSF1 *font = (PSF1*) &_binary_font_psf_start;
    MainRenderer = Renderer(framebuffer, font, 0xFFFFFFFF, 0x00000000);

    MemoryMap memoryMap;
    memoryMap.Entries = (MemoryMapEntry*) (&bootboot.mmap);
    memoryMap.EntryCount = (bootboot.size - sizeof(BOOTBOOT) + sizeof(MMapEnt)) / sizeof(MemoryMapEntry);
    memoryMap.MemorySizeKB = memoryMap.Entries[memoryMap.EntryCount - 1].Address + MemoryMapEntry_Size(memoryMap.Entries[memoryMap.EntryCount - 1]);
    memoryMap.MemorySizeKB /= 1024;

    #ifdef LOGGING
    if(InitializeSerialPort(SERIAL_COM1) == -1)
    {
        MainRenderer.PrintErrorf("SERIAL PORT COM 1 INITIALIZATION FAILURE.\n");
        while(1);
    }
    Logf("Serial port COM1 initialized for logging.\n\n");
    Logf("******************************************************************************************\n\n");
    #endif

    // Load GDT
    GDTR gdtr;
    gdtr.Size = sizeof(GDT) - 1;
    gdtr.PhysicalAddress = (uint64_t) &GlobalDescriptorTable;
    LoadGDT(&gdtr);
    #ifdef LOGGING
    Logf("GDT Loaded.\n");
    #endif

    // Initialize Page Frame Allocator.
    FrameAllocator.Initialize(memoryMap);
    #ifdef LOGGING
    Logf("Page Frame Allocator initialized.\nFree memory = 0x%x\nUsed memory = 0x%x\nReserved memory = 0x%x\n", 
        FrameAllocator.FreeMemory, FrameAllocator.UsedMemory, FrameAllocator.ReservedMemory);
    #endif

    PageTable *pageTableL4;
    asm volatile("mov %%cr3, %%rax" : "=a"(pageTableL4) : );
    PagingManager = PageTableManager(pageTableL4);
    #ifdef LOGGING
    Logf("Kernel Page Table Manager initialized.\n");
    #endif

    InitializeInterrupts();
    #ifdef LOGGING
    Logf("Interrupts initialized.\n\n");
    #endif

    InitializeHeap((void*) HEAP_ADDRESS, 16);
    #ifdef LOGGING
    Logf("Heap initialized.\n\n");
    #endif

    KernelStart();
}