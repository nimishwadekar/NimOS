#pragma once

#include <Syscalls/Syscall.hpp>

void SysGetFore(Registers *regs);
void SysSetFore(Registers *regs);
void SysGetBack(Registers *regs);
void SysSetBack(Registers *regs);
void SysGetRes(Registers *regs);
void SysGetCrsr(Registers *regs);
void SysSetCrsr(Registers *regs);
void SysDrawP(Registers *regs);
void SysDrawL(Registers *regs);
void SysDrawR(Registers *regs);
void SysLockScr(Registers *regs);
void SysUnlockScr(Registers *regs);
void SysScrScrolled(Registers *regs);