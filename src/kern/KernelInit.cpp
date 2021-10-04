#include <stdint.h>
#include <bootboot.h>
#include <ACPI/ACPI.hpp>
#include <DateTime.hpp>
#include <Display/Framebuffer.hpp>
#include <Display/Renderer.hpp>
#include <Environment.hpp>
#include <FS/VFS.hpp>
#include <Fonts/PSF.hpp>
#include <GDT.hpp>
#include <IO/PIT.hpp>
#include <IO/Serial.hpp>
#include <Interrupts/Interrupts.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/Memory.hpp>
#include <Memory/MemoryMap.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Memory/Paging.hpp>
#include <PCI/PCI.hpp>
#include <Storage/DiskInfo.hpp>
#include <String.hpp>
#include <Tasking/Process.hpp>
#include <Usermode/ArgV.hpp>

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

extern volatile unsigned char _binary_font_psf_start;

extern void KernelStart(void);

#define HEAP_ADDRESS 0xFFFFFFFF00000000

void SetupACPI(const uint64_t xsdtAddress);

// Entry point into kernel, called by Bootloader.
void main()
{
    GDTR gdtr;
    gdtr.Size = sizeof(GDT) - 1;
    gdtr.PhysicalAddress = (uint64_t) &GlobalDescriptorTable;
    LoadGDT(&gdtr);
    Environment.ParseEnvironemnt(environment);
    InitializeInterrupts();

    #ifdef LOGGING
    if(InitializeSerialPort(SERIAL_COM1) == -1)
    {
        errorf("SERIAL PORT COM 1 INITIALIZATION FAILURE.\n");
        while(1);
    }
    logf("Serial port COM1 initialized for logging.\n\n");
    logf("******************************************************************************************\n\n");
    #endif

    Framebuffer framebuffer((uint32_t*) &fb, (Framebuffer::FBType) bootboot.fb_type, 
        bootboot.fb_size, (int32_t) bootboot.fb_width, (int32_t) bootboot.fb_height, (int32_t) bootboot.fb_scanline >> 2);
    PSF1 *font = (PSF1*) &_binary_font_psf_start;
    MainRenderer = Renderer(framebuffer, font, COLOUR_BLACK, COLOUR_WHITE);
    MainRenderer.ClearScreen();

    MemoryMap memoryMap;
    memoryMap.Entries = (MemoryMapEntry*) (&bootboot.mmap);
    memoryMap.EntryCount = (bootboot.size - sizeof(BOOTBOOT) + sizeof(MMapEnt)) / sizeof(MemoryMapEntry);
    memoryMap.MemorySizeKB = memoryMap.Entries[memoryMap.EntryCount - 1].Address + MemoryMapEntry_Size(memoryMap.Entries[memoryMap.EntryCount - 1]);
    memoryMap.MemorySizeKB /= 1024;

    // Initialize Page Frame Allocator.
    FrameAllocator.Initialize(memoryMap);

    PageTable *pageTableL4;
    asm volatile("mov %%cr3, %%rax" : "=a"(pageTableL4) : );
    PagingManager = PageTableManager(pageTableL4);

    KernelHeap.InitializeHeap((void*) HEAP_ADDRESS, 16);
    SetupACPI(bootboot.arch.x86_64.acpi_ptr);
    PIT::SetDivisor(20000);
    DiskInformation.Initialize();
    VFSInitialize(&DiskInformation);

    // Load TSS entry offset in GDT.
    asm volatile("ltr %%ax" : : "r"((6 * 8) | 0));

    InitializeProcessManager();
    InitArgvBuffer();
    
    KernelStart();
}

void SetupACPI(const uint64_t xsdtAddress)
{
    ACPI::SDTHeader *xsdtHeader = (ACPI::SDTHeader*) xsdtAddress;

    int signatureValid = memcmp(xsdtHeader->Signature, "XSDT", 4);
    if(signatureValid != 0)
    {
        errorf("XSDT Table not found.");
        while(true);
    }

    if(!ACPI::IsChecksumValid(xsdtHeader))
    {
        errorf("XSDT Table checksum not valid.");
        while(true);
    }

    ACPI::MCFGHeader *mcfgHeader = (ACPI::MCFGHeader*) ACPI::FindTable(xsdtHeader, "MCFG");
    if(mcfgHeader == NULL)
    {
        errorf("MCFG Table not found.");
        while(true);
    }
    
    PCI::EnumeratePCI(mcfgHeader);
}