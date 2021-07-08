#include "IDT.hpp"

IDTR IDTRegister;
IDTEntry IDT[0x30];

void IDTEntry::SetOffset(uint64_t offset)
{
    Offset0 = (uint16_t) (offset & 0xFFFF);
    offset >>= 16;
    Offset1 = (uint16_t) (offset & 0xFFFF);
    offset >>= 16;
    Offset2 = (uint32_t) (offset);
}

uint64_t IDTEntry::GetOffset(void)
{
    uint64_t offset = 0;
    offset |= (uint64_t) Offset0;
    offset |= (uint64_t) Offset1 << 16;
    offset |= (uint64_t) Offset2 << 32;
    return offset;
}