#include "Paging.hpp"

void PageTableEntry::SetFlag(PageTableFlags flag, bool enabled)
{
    uint64_t bit = 1 << (int) flag;
    if(enabled) Value |= bit;
    else Value &= ~(bit);
}

bool PageTableEntry::GetFlag(PageTableFlags flag)
{
    uint64_t bit = 1 << (int) flag;
    return Value & bit;
}

void PageTableEntry::SetAddress(uint64_t address)
{
    address &= 0x000FFFFFFFFFF000;
    Value &= 0xFFF0000000000FFF;
    Value |= address;
}

uint64_t PageTableEntry::GetAddress(void)
{
    return (Value & 0x000FFFFFFFFFF000);
}