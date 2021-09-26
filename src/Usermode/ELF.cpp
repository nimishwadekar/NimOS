#include <Display/Renderer.hpp>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Usermode/ELF.hpp>

namespace ELF
{
    void *LoadELF(void *fileAddress)
    {
        Header *header = (Header*) fileAddress;
        if(header->MagicNumber != ELF_MAGIC || memcmp(header->Signature, "ELF", 3) != 0) return nullptr;
        if(header->Format != 2 || header->ByteOrdering != 1) return nullptr;
        if(header->Type != ELF_TYPE_EXE || header->InstructionSet != ELF_INSTR_SET_X86_64) return nullptr;

        ProgramHeader *progHeader = (ProgramHeader*)((uint8_t*) fileAddress + header->ProgramHeaderTablePosition);
        for(uint16_t i = 0; i < header->ProgramHeaderCount; i++, progHeader++)
        {
            if(progHeader->Type != ELF_HDR_TYPE_LOAD) continue;

            //uint64_t pageStart = progHeader->P_Vaddr - (progHeader->P_Vaddr % 0x1000);
            uint64_t pages = (progHeader->P_Vaddr % 0x1000 + progHeader->P_MemSz + 0xFFF) / 0x1000;
            for(uint64_t p = 0; p < pages; p++) PagingManager.MapPage((uint8_t*) progHeader->P_Vaddr + p * 0x1000, FrameAllocator.RequestPageFrame());
            memcpy((uint8_t*) fileAddress + progHeader->P_Offset, (void*) progHeader->P_Vaddr, progHeader->P_FileSz);
            if(progHeader->P_FileSz < progHeader->P_MemSz) 
                memset((uint8_t*) progHeader->P_Vaddr + progHeader->P_FileSz, 0, progHeader->P_MemSz - progHeader->P_FileSz);
        }

        return (void*) header->ProgramEntryPosition;

        while(1);
    }
}