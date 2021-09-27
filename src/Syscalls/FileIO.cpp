#include <FS/VFS.hpp>
#include <Syscalls/FileIO.hpp>

void SysFOpen(Registers *regs)
{
    const char *mode = 0;
    switch(regs->RDX)
    {
        case 0: mode = "r"; break;
        case 1: mode = "w"; break;
        case 2: mode = "r+"; break;
        default: regs->RAX = ERR_BAD_MODE; return;
    }

    FILE *file = VFSOpenFile((const char*) regs->RSI, mode);
    if(!file) regs->RAX = ERR_FILE_NOT_FOUND_RD;
    else regs->RAX = file->Handle;
}


void SysFClose(Registers *regs)
{
    regs->RAX = VFSCloseFile(&OpenedFiles[regs->RDX]);
}


void SysFRead(Registers *regs)
{
    regs->RAX = VFSReadFile(&OpenedFiles[regs->RDX], (void*) regs->RDI, regs->R10);
}


void SysFWrite(Registers *regs)
{
    regs->RAX = VFSWriteFile(&OpenedFiles[regs->RDX], (void*) regs->RSI, regs->R10);
}


void SysFSeek(Registers *regs)
{
    regs->RAX = VFSSeekFile(&OpenedFiles[regs->RDX], regs->R10, regs->R8);
}


void SysFTell(Registers *regs)
{
    regs->RAX = VFSTellFile(&OpenedFiles[regs->RDX]);
}