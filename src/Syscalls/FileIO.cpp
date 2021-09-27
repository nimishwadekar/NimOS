#include <FS/VFS.hpp>
#include <Syscalls/FileIO.hpp>

void SysFOpen(Registers *regs)
{
    const char *mode = 0;
    switch(regs->RSI)
    {
        case 0: mode = "r"; break;
        case 1: mode = "w"; break;
        case 2: mode = "r+"; break;
        default: regs->RAX = ERR_BAD_MODE; return;
    }

    FILE *file = VFSOpenFile((const char*) regs->RDI, mode);
    if(!file) regs->RAX = ERR_FILE_NOT_FOUND_RD;
    else regs->RAX = file->Handle;
}


void SysFClose(Registers *regs)
{
    regs->RAX = VFSCloseFile(&OpenedFiles[regs->RDI]);
}


void SysFRead(Registers *regs)
{
    regs->RAX = VFSReadFile(&OpenedFiles[regs->RDI], (void*) regs->RSI, regs->RDX);
}


void SysFWrite(Registers *regs)
{
    regs->RAX = VFSWriteFile(&OpenedFiles[regs->RDI], (void*) regs->RSI, regs->RDX);
}


void SysFSeek(Registers *regs)
{
    regs->RAX = VFSSeekFile(&OpenedFiles[regs->RDI], regs->RSI, regs->RDX);
}


void SysFTell(Registers *regs)
{
    regs->RAX = VFSTellFile(&OpenedFiles[regs->RDI]);
}