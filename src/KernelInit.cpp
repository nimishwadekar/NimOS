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

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

extern volatile unsigned char _binary_font_psf_start;

extern void KernelStart(void);

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
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.Size = sizeof(GDT) - 1;
    gdtDescriptor.PhysicalAddress = (uint64_t) &GlobalDescriptorTable;
    LoadGDT(&gdtDescriptor);
    #ifdef LOGGING
    Logf("GDT Loaded.\n");
    #endif

    // Initialize Page Frame Allocator.
    FrameAllocator.Initialize(memoryMap);
    #ifdef LOGGING
    Logf("Free memory = 0x%x\nUsed memory = 0x%x\nReserved memory = 0x%x\n", 
        FrameAllocator.FreeMemory, FrameAllocator.UsedMemory, FrameAllocator.ReservedMemory);
    #endif

    KernelStart();
}