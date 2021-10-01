#ifndef _ELF64_H
#define _ELF64_H 1

#include <stdint.h>

#define EI_64IDENT 16

#define EI_MAG0         0x7F
#define EI_MAG1         'E'
#define EI_MAG2         'L'
#define EI_MAG3         'F'
#define ELFCLASS64      2
#define ELFDATA2LSB     1
#define ELFOSABI_SYSV   0

#define ET_REL          1
#define ET_EXEC         2
#define ET_DYN          3
#define ET_CORE         4

#define EM_386          3
#define EM_X86_64       0x3E

typedef struct 
{
    unsigned char   e_ident[EI_64IDENT];

    uint16_t        e_type;
    uint16_t        e_machine;
    uint32_t        e_version;
    uint64_t        e_entry;

    uint64_t        e_phoff;
    uint64_t        e_shoff;
    uint32_t        e_flags;

    uint16_t        e_ehsize;
    uint16_t        e_phentsize;
    uint16_t        e_phnum;

    uint16_t        e_shentsize;
    uint16_t        e_shnum;
    uint16_t        e_shstrndx;
} Elf64_Ehdr;


#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2
#define PT_INTERP       3
#define PT_NOTE         4

#define PF_X            1
#define PF_W            2
#define PF_R            4

typedef struct 
{
    uint32_t        p_type;
    uint32_t        p_flags;
    uint64_t        p_offset;
    uint64_t        p_vaddr;
    uint64_t        p_paddr;
    uint64_t        p_filesz;
    uint64_t        p_memsz;
    uint64_t        p_align;
} Elf64_Phdr;

#endif