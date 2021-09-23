#include <stddef.h>
#include "ACPI.hpp"
#include "../Memory/Memory.hpp"

namespace ACPI
{
    bool IsChecksumValid(const ACPI::SDTHeader *header)
    {
        uint8_t *bytes = (uint8_t*) header;
        uint8_t sum = 0;
        for(uint32_t i = 0; i < header->Length; i++) sum += bytes[i];
        return sum == 0;
    }

    void *FindTable(const ACPI::SDTHeader *header, const char *signature)
    {
        uint32_t entryCount = (header->Length - sizeof(ACPI::SDTHeader)) / 8;
        ACPI::SDTHeader **entries = (ACPI::SDTHeader**) ((uint64_t) header + sizeof(ACPI::SDTHeader));
        for(uint32_t i = 0; i < entryCount; i++)
        {
            if(memcmp(entries[i]->Signature, signature, 4) == 0) return entries[i];
        }
        return NULL;
    }
}