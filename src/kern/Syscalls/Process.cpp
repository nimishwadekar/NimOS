#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <String.hpp>
#include <Syscalls/Process.hpp>
#include <Tasking/Process.hpp>
#include <Tasking/SharedMemory.hpp>
#include <Usermode/ArgV.hpp>
#include <Usermode/ELF.hpp>
#include <Usermode/Usermode.hpp>

#include <Display/Renderer.hpp>
#include <Memory/PageMapIndexer.hpp>

/**
 *     DUPLICATE PAGE TABLES
 */ 

extern uint64_t STATE_RIP;
extern uint64_t STATE_RBX;
extern uint64_t STATE_RBP;
extern uint64_t STATE_RSP;
extern uint64_t STATE_R12;
extern uint64_t STATE_R13;
extern uint64_t STATE_R14;
extern uint64_t STATE_R15;
extern uint64_t STATE_RFLAGS;


void SysExec(Registers *regs)
{
    char **argv = (char**) regs->RSI;
    int argc = 0;
    char *argbuf = ArgvBuffer;
    while(*argv)
    {
        if(argbuf - ArgvBuffer >= ARGV_BUFFER_SIZE) break;
        strcpy(*argv, argbuf);
        argbuf += strlen(*argv) + 1;
        argv++;
        argc++;
    }

    ELF::LoadInfo info = ELF::LoadELF((void*) regs->RDI);
    PopProcess();
    if(PushProcess(info.Entry, info.FirstAddress, info.PageCount) == -1)
    {
        regs->RAX = (uint64_t) -1;
        return;
    }
    Process *newProcess = PeekProcess();

    uint64_t *stack = (uint64_t*) newProcess->StackTop - (argc + 2);
    *stack = argc;
    argbuf = ArgvBuffer;
    for(int i = 1; i <= argc; i++)
    {
        *(stack + i) = (uint64_t) argbuf;
        argbuf += strlen(argbuf) + 1;
    }
    *(stack + argc + 1) = 0;
    newProcess->StackTop = stack;

    JumpToUserAddress_Syscall(stack, newProcess->HeapBase, info.Entry);
}


void SysSpawn(Registers *regs)
{
    char **argv = (char**) regs->RSI;
    int argc = 0;
    char *argbuf = ArgvBuffer;
    while(*argv)
    {
        if(argbuf - ArgvBuffer >= ARGV_BUFFER_SIZE) break;
        strcpy(*argv, argbuf);
        argbuf += strlen(*argv) + 1;
        argv++;
        argc++;
    }

    Process *oldProcess = PeekProcess();
    oldProcess->PC = (void*) STATE_RIP;
    oldProcess->Regs.R12 = STATE_R12;
    oldProcess->Regs.R13 = STATE_R13;
    oldProcess->Regs.R14 = STATE_R14;
    oldProcess->Regs.R15 = STATE_R15;
    oldProcess->Regs.RBP = STATE_RBP;
    oldProcess->Regs.RBX = STATE_RBX;
    oldProcess->Regs.RFLAGS = STATE_RFLAGS;
    oldProcess->Regs.RSP = STATE_RSP;
    oldProcess->SpawnExitCodeAddr = (int*) regs->RDX;

    BackupTopProcess();

    ELF::LoadInfo info = ELF::LoadELF((void*) regs->RDI);
    if(PushProcess(info.Entry, info.FirstAddress, info.PageCount) == -1)
    {
        regs->RAX = (uint64_t) -1;
        return;
    }
    Process *newProcess = PeekProcess();

    uint64_t *stack = (uint64_t*) newProcess->StackTop - (argc + 2);
    *stack = argc;
    argbuf = ArgvBuffer;
    for(int i = 1; i <= argc; i++)
    {
        *(stack + i) = (uint64_t) argbuf;
        argbuf += strlen(argbuf) + 1;
    }
    *(stack + argc + 1) = 0;
    newProcess->StackTop = stack;

    JumpToUserAddress_Syscall(stack, newProcess->HeapBase, info.Entry);
}


void SysExit(Registers *regs)
{
    PopProcess();
    RestoreTopProcess();

    Process *p = PeekProcess();
    STATE_RIP = (uint64_t) p->PC;
    STATE_R12 = p->Regs.R12;
    STATE_R13 = p->Regs.R13;
    STATE_R14 = p->Regs.R14;
    STATE_R15 = p->Regs.R15;
    STATE_RBP = p->Regs.RBP;
    STATE_RBX = p->Regs.RBX;
    STATE_RFLAGS = p->Regs.RFLAGS;
    STATE_RSP = p->Regs.RSP;

    if(p->SpawnExitCodeAddr) *(p->SpawnExitCodeAddr) = (int) regs->RDI;
}


void SysPID(Registers *regs)
{
    regs->RAX = PeekProcess()->ProcessID;
}


void SysPPID(Registers *regs)
{
    regs->RAX = (ProcessTop - 2)->ProcessID;
}


void SysShmAt(Registers *regs)
{
    Process *p = PeekProcess();
    if(regs->RDI >= SHM_MAX_SEGS || p->SharedMemKey == (int16_t) regs->RDI)
    {
        regs->RAX = 0;
        return;
    }

    if(p->SharedMemKey != -1)
    {
        ShmManager.DetachProcess((uint8_t) p->SharedMemKey);
    }
    p->SharedMemKey = (uint16_t) regs->RDI;
    regs->RAX = (uint64_t) ShmManager.AttachProcess((uint8_t) regs->RDI);
}


void SysShmDt(Registers *regs)
{
    Process *p = PeekProcess();
    if(p->SharedMemKey == -1)
    {
        regs->RAX = (uint64_t) -1;
        return;
    }

    ShmManager.DetachProcess((uint8_t) p->SharedMemKey);
    p->SharedMemKey = -1;
    regs->RAX = 0;
}