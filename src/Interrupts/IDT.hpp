#pragma once

#include <stdint.h>

#define IDT_TA_InterruptGate    0b10001110
#define IDT_TA_CallGate         0b10001100
#define IDT_TA_TrapGate         0b10001111

struct IDTR
{
    uint16_t Limit;
    uint64_t PhysicalAddress;
} __attribute__((packed));

struct IDTEntry
{
    uint16_t Offset0;
    uint16_t Selector;
    uint8_t IST;
    uint8_t Type_Attribute;
    uint16_t Offset1;
    uint32_t Offset2;
    const uint32_t ZERO;

    void SetOffset(uint64_t offset);
    uint64_t GetOffset(void);
} __attribute__((packed));

extern IDTR IDTRegister;