#pragma once
#include <stdint.h>

struct TSS
{
    uint32_t Reserved0;
    uint64_t RSP[3];
    uint64_t Reserved1;
    uint64_t IST[7];
    uint64_t Reserved2;
    uint16_t Reserved3;
    uint16_t IOMapBaseAddress;
} __attribute__((packed));

struct GDTR
{
    uint16_t Size;
    uint64_t PhysicalAddress;
} __attribute__((packed));

struct GDTEntry
{
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
} __attribute__((packed));

struct GDT
{
    GDTEntry NullEntry;
    GDTEntry KernelCode;
    GDTEntry KernelData;
    GDTEntry UserCode;
    GDTEntry UserData;
    GDTEntry UserCode2; // Duplicate of UserCode for Sysret.
    GDTEntry TSSEntry;
} __attribute__((packed)) 
__attribute__((aligned(0x1000)));

extern TSS TaskStateSegment;
extern GDT GlobalDescriptorTable;

extern "C" void LoadGDT(GDTR *gdtr);