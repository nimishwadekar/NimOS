#pragma once

#include <stdint.h>

enum class PageTableFlags
{
    Present = 0,
    ReadWrite = 1,
    UserAccess = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    HugePage = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // only if supported
};

struct PageTableEntry
{
    uint64_t Value;
    void SetFlag(PageTableFlags flag, bool enabled);
    bool GetFlag(PageTableFlags flag);
    void SetAddress(uint64_t address);
    uint64_t GetAddress(void);
};

struct PageTable
{
    PageTableEntry Entries[512];
} __attribute__((aligned(0x1000)));