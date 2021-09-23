#pragma once

#include <stdint.h>

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
} __attribute__((packed)) 
__attribute__((aligned(0x1000)));

extern GDT GlobalDescriptorTable;

extern "C" void LoadGDT(GDTR *gdtr);