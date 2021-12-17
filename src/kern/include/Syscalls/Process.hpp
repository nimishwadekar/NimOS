#pragma once

#include <Syscalls/Syscall.hpp>

void SysExec(Registers *regs);
void SysSpawn(Registers *regs);
void SysExit(Registers *regs);
void SysPID(Registers *regs);
void SysPPID(Registers *regs);
void SysShmAt(Registers *regs);
void SysShmDt(Registers *regs);