#pragma once

#include <Syscalls/Syscall.hpp>

void SysPrintN(Registers *regs);
void SysPrint(Registers *regs);
void SysInputN(Registers *regs);
void SysInput(Registers *regs);