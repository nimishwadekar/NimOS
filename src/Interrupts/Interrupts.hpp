#pragma once

#include <stdint.h>
#include "IDT.hpp"

struct InterruptFrame
{
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFlags;
    uint64_t RSP;
};

struct ExceptionFrame
{
    uint64_t ErrorCode;
    uint64_t RIP;
    uint64_t CS;
    uint64_t RFlags;
    uint64_t RSP;
};

void InitializeInterrupts(void);