#pragma once

#include <Syscalls/Syscall.hpp>

#define ERR_BAD_MODE -1
#define ERR_FILE_NOT_FOUND_RD -2

void SysFOpen(Registers *regs);
void SysFClose(Registers *regs);
void SysFRead(Registers *regs);
void SysFWrite(Registers *regs);
void SysFSeek(Registers *regs);
void SysFTell(Registers *regs);