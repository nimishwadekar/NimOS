#pragma once

#include <stdint.h>
#include <Usermode/Usermode.hpp>

extern "C" void SyscallEntry(void);

struct Registers
{
    uint64_t RDI;
    uint64_t RSI;
    uint64_t RDX;
    uint64_t R10;
    uint64_t R8;
    uint64_t R9;
    uint64_t RAX;
};


// STDIN, STDOUT System Calls

#define SYS_PRINTN      0x01
// Prints n bytes.
// rsi = string, r10 = n

#define SYS_PRINT       0x02
// Prints till null character.
// rsi = string

#define SYS_INPUTN      0x03
// Inputs n characters and stores at location.
// rdi = location, r10 = n

#define SYS_INPUT       0x04
// Inputs characters till Enter key is pressed. Enter is included in the input data.
// rdi = location