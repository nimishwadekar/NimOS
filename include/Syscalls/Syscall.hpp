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
// rdi = string
// rsi = n

#define SYS_PRINT       0x02
// Prints till null character.
// rdi = string

#define SYS_INPUTN      0x03
// Inputs n characters and stores at location.
// rdi = location
// rsi = n

#define SYS_INPUT       0x04
// Inputs characters till Enter key is pressed. Enter is included in the input data.
// rdi = location


// File IO System Calls

#define SYS_FOPEN       0x11
// Opens file.
// rdi - Null terminated file name string
// rsi - Mode (0 - read, 1 - write, 2 - both)
// Return value - File descriptor/Error code(-1 wrong mode, -2 no file to read)

#define SYS_FCLOSE      0x12
// Closes an open file.
// rdi - File descriptor

#define SYS_FREAD       0x13
// Reads n bytes from an open file.
// rdi - File descriptor
// rsi - Destination
// r10 - n

#define SYS_FWRITE      0x14
// Writes n bytes to an open file.
// rdi - File descriptor
// rsi - Source
// r10 - n

#define SYS_FSEEK       0x15
// Seeks position in file.
// rdi - File descriptor
// rsi - Offset
// r10 - Whence (0 - set, 1 - cur, 2 - end)

#define SYS_FTELL       0x16
// Tells position in file.
// rdi - File descriptor
// Return value - position in file


// Execution System Calls

#define SYS_EXEC        0x21
// Executes program.
// rdi - Null terminated file name string
// rsi - argv
// r10 - envp

#define SYS_EXECR       0x22
// Executes program and returns after finishing.
// rdi - Null terminated file name string
// rsi - argv
// r10 - envp


// Memory System Calls

#define SYS_MMAP        0x41
// Allocates pages dynamically.
// rdi - Virtual address
// rsi - Number of pages
// r10 - 0 (may be non-contiguous), 1 (always contiguous)


// Time System Calls

#define SYS_SLEEP       0x51
// Sleeps for a specified period of time.
// rdi - Period
// rsi - 0 - seconds, 1 - milliseconds


// Display System Calls

#define SYS_GETFORE     0x81
// Gets foreground colour.
// Return value - Foregound colour

#define SYS_SETFORE     0x82
// Sets foreground colour.
// rdi - colour

#define SYS_GETBACK     0x83
// Gets background colour.
// Return value - Background colour

#define SYS_SETBACK     0x84
// Sets background colour. Also clears the screen.
// rdi - colour

#define SYS_GETRES      0x85
// Gets screen resolution.
// Return value - resolution: rax[63:32] - width, rax[31:0] - height

#define SYS_GETCRSR     0x86
// Gets cursor position.
// Return value - position: rax[63:32] - x, rax[31:0] - y

#define SYS_SETCRSR     0x87
// Sets cursor position.
// rdi - x
// rsi - y

#define SYS_DRAWP       0x88
// Draws a pixel.
// rdi - x
// rsi - y
// r10 - colour

#define SYS_DRAWL       0x89
// Draws a line.
// rdi - x1
// rsi - y1
// r10 - x2
// rdx - y2
// r8 - colour

#define SYS_DRAWR       0x8A
// Draws a rectangle.
// rdi - x
// rsi - y
// r10 - width
// rdx - height
// r8 - colour