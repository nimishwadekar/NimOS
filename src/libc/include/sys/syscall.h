#ifndef _SYSCALL_H
#define _SYSCALL_H 1

#include <stdint.h>

// _syscall_X : Syscall with X arguments.

int64_t _syscall_0(int64_t syscall);
int64_t _syscall_1(int64_t syscall, int64_t arg0);
int64_t _syscall_2(int64_t syscall, int64_t arg0, int64_t arg1);
int64_t _syscall_3(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2);
int64_t _syscall_4(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3);
int64_t _syscall_5(int64_t syscall, int64_t arg0, int64_t arg1, int64_t arg2, int64_t arg3, int64_t arg4);


// Syscall numbers.

#define SYS_PRINTN      0x01
#define SYS_PRINT       0x02
#define SYS_INPUTN      0x03
#define SYS_INPUT       0x04

#define SYS_FOPEN       0x11
#define SYS_FCLOSE      0x12
#define SYS_FREAD       0x13
#define SYS_FWRITE      0x14
#define SYS_FSEEK       0x15
#define SYS_FTELL       0x16

#define SYS_EXEC        0x21
#define SYS_SPAWN       0x22
#define SYS_EXIT        0x23
#define SYS_PID         0x24
#define SYS_PPID        0x25
#define SYS_SHMAT       0x26
#define SYS_SHMDT       0x27

#define SYS_MMAP        0x41

#define SYS_SLEEP       0x51
#define SYS_TIME        0x52

#define SYS_GETFORE     0x81
#define SYS_SETFORE     0x82
#define SYS_GETBACK     0x83
#define SYS_SETBACK     0x84
#define SYS_GETRES      0x85
#define SYS_GETCRSR     0x86
#define SYS_SETCRSR     0x87
#define SYS_DRAWP       0x88
#define SYS_DRAWL       0x89
#define SYS_DRAWR       0x8A
#define SYS_LOCKSCR     0x8B
#define SYS_UNLOCKSCR   0x8C
#define SYS_SCRSCROLLED 0x8D

#endif