#pragma once

#include <stdint.h>
#include <Interrupts/IDT.hpp>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

#define PIC_EOI 0x20

struct InterruptFrame
{
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFlags;
    uint64_t RSP;
};

/* struct ExceptionFrame
{
    uint64_t ErrorCode;
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFlags;
    uint64_t RSP;
}; */

struct ExceptionFrame
{
    uint64_t ErrorCode;
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFlags;
    uint64_t RSP;
};

void InitializeInterrupts(void);

// Interrupt values = 0-16.
void PICUnmask(uint8_t interrupt);

// Interrupt values = 0-16.
void PICMask(uint8_t interrupt);

// Interrupt values = 0-16.
void PICEndOfInterrupt(const uint8_t interrupt);