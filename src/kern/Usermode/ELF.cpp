#include <Display/Renderer.hpp>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Usermode/ELF.hpp>

namespace ELF
{
    LoadInfo LoadELF(void *fileAddress)
    {
        Header *header = (Header*) fileAddress;
        if(header->MagicNumber != ELF_MAGIC || memcmp(header->Signature, "ELF", 3) != 0) return {nullptr, 0, 0};
        if(header->Format != 2 || header->ByteOrdering != 1) return {nullptr, 0, 0};
        if(header->Type != ELF_TYPE_EXE || header->InstructionSet != ELF_INSTR_SET_X86_64) return {nullptr, 0, 0};

        uint64_t firstAddr = 0;
        uint64_t lastAddr = 0;

        ProgramHeader *progHeader = (ProgramHeader*)((uint8_t*) fileAddress + header->ProgramHeaderTablePosition);
        for(uint16_t i = 0; i < header->ProgramHeaderCount; i++, progHeader++)
        {
            if(progHeader->Type != ELF_HDR_TYPE_LOAD) continue;

            uint64_t pages = (progHeader->P_Vaddr % 0x1000 + progHeader->P_MemSz + 0xFFF) / 0x1000;

            if(firstAddr == 0) firstAddr = progHeader->P_Vaddr;
            if(lastAddr < progHeader->P_Vaddr + progHeader->P_MemSz) lastAddr = progHeader->P_Vaddr + progHeader->P_MemSz;

            uint8_t *addr = (uint8_t*) FrameAllocator.RequestPageFrames(pages);
            for(uint64_t p = 0; p < pages; p++)
            {
                PagingManager.MapPage((uint8_t*) progHeader->P_Vaddr + p * 0x1000, addr + p * 0x1000);
            }

            memcpy((uint8_t*) fileAddress + progHeader->P_Offset, (void*) progHeader->P_Vaddr, progHeader->P_FileSz);
            if(progHeader->P_FileSz < progHeader->P_MemSz) 
                memset((uint8_t*) progHeader->P_Vaddr + progHeader->P_FileSz, 0, progHeader->P_MemSz - progHeader->P_FileSz);
        }

        LoadInfo info;
        info.Entry = (void*) header->ProgramEntryPosition;
        info.FirstAddress = firstAddr & (~0xFFF);
        info.PageCount = (((lastAddr & (~0xFFF)) + 0x1000) - info.FirstAddress) / 0x1000;
        return info;
    }
}