#pragma once

#include <stdint.h>

#define MMAP_USED 0
#define MMAP_FREE 1
#define MMAP_ACPI 2
#define MMAP_MMIO 3

// Extracts the size of the memory map entry.
#define MemoryMapEntry_Size(entry) (entry.Size_Type & 0xFFFFFFFFFFFFFFF0)

// Extracts the type of the memory map entry.
#define MemoryMapEntry_Type(entry) (entry.Size_Type & 0x000000000000000F)

extern const char *MemoryTypeStrings[];

// Gets the string representation of the type of the memory map entry.
#define MemoryMapEntry_TypeString(entry) (MemoryTypeStrings[MemoryMapEntry_Type(entry)])

struct MemoryMapEntry
{
    uint64_t Address; // Base address of the memory chunk.
    uint64_t Size_Type; // Lowest 4 bits for type, rest for 16-byte aligned size.
} __attribute__((packed));

struct MemoryMap
{
    MemoryMapEntry *Entries; // Pointer to first memory map entry.
    unsigned int EntryCount; // Number of memory map entries.
    unsigned long MemorySizeKB; // Total size of memory in KB;
};