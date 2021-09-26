#pragma once

#include <stdint.h>

namespace ELF
{
    #define ELF_MAGIC 0x7F

    #define ELF_TYPE_RELOC 1
    #define ELF_TYPE_EXE 2
    #define ELF_TYPE_SHARED 3
    #define ELF_TYPE_CORE 4

    #define ELF_INSTR_SET_NONE 0
    #define ELF_INSTR_SET_X86 3
    #define ELF_INSTR_SET_X86_64 0x3E

    struct Header
    {
        uint8_t MagicNumber; // 0x7F.
        char Signature[3]; // "ELF".
        uint8_t Format; // 1 = 32-bit, 2 = 64-bit.
        uint8_t ByteOrdering; // 1 = little endian, 2 - big endian.
        uint8_t HeaderVersion;
        uint8_t OSABI;
        uint8_t Reserved0[8];

        uint16_t Type; // 1 = Relocatable, 2 = Executable, 3 = Shared, 4 = Core.
        uint16_t InstructionSet;
        uint32_t ELFVersion;
        uint64_t ProgramEntryPosition;

        uint64_t ProgramHeaderTablePosition;
        uint64_t SectionHeaderTablePosition;
        uint32_t Flags;
        uint16_t HeaderSize;
        uint16_t ProgramHeaderSize;
        uint16_t ProgramHeaderCount;
        uint16_t SectionHeaderSize;
        uint16_t SectionHeaderCount;
        uint16_t SectionHeaderTableIndex; // With Section Header names.
    } __attribute__((packed));

    #define ELF_HDR_TYPE_NULL 0
    #define ELF_HDR_TYPE_LOAD 1
    #define ELF_HDR_TYPE_DYNAMIC 2
    #define ELF_HDR_TYPE_INTERP 3
    #define ELF_HDR_TYPE_NOTE 4

    #define ELF_HDR_FLAG_EXE 1
    #define ELF_HDR_FLAG_WR 2
    #define ELF_HDR_FLAG_RD 4

    struct ProgramHeader
    {
        uint32_t Type;
        uint32_t Flags;
        uint64_t P_Offset;
        uint64_t P_Vaddr;
        uint64_t Undefined;
        uint64_t P_FileSz;
        uint64_t P_MemSz;
        uint64_t Alignment;
    } __attribute__((packed));

    // Loads an ELF File into memory. Returns the start location of the program.
    void *LoadELF(void *fileAddress);
}