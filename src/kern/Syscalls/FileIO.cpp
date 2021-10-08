#include <FS/VFS.hpp>
#include <Syscalls/FileIO.hpp>
#include <Tasking/Process.hpp>

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
    if(!file)
    {
        regs->RAX = ERR_FILE_NOT_FOUND_RD;
        return;
    }
    else regs->RAX = file->Handle;
    AddFileToCurrentProcess(file);
}


void SysFClose(Registers *regs)
{
    RemoveFileFromCurrentProcess(regs->RDI);
    regs->RAX = VFSCloseFile(OpenedFiles.Array[regs->RDI]);
}


void SysFRead(Registers *regs)
{
    regs->RAX = VFSReadFile(OpenedFiles.Array[regs->RDI], (void*) regs->RSI, regs->RDX);
}


void SysFWrite(Registers *regs)
{
    regs->RAX = VFSWriteFile(OpenedFiles.Array[regs->RDI], (void*) regs->RSI, regs->RDX);
}


void SysFSeek(Registers *regs)
{
    regs->RAX = VFSSeekFile(OpenedFiles.Array[regs->RDI], regs->RSI, regs->RDX);
}


void SysFTell(Registers *regs)
{
    regs->RAX = VFSTellFile(OpenedFiles.Array[regs->RDI]);
}