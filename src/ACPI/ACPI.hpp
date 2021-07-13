#pragma once

#include <stdint.h>

namespace ACPI
{
    // System Descriptor Table Header.
    struct SDTHeader
    {
        char Signature[4];
        uint32_t Length;
        uint8_t Revision;
        uint8_t Checksum;
        char OEMID[6];
        char OEMTableID[8];
        uint32_t OEMRevision;
        uint32_t CreatorID;
        uint32_t CreatorRevision;
    } __attribute__((packed));

    struct MCFGHeader
    {
        ACPI::SDTHeader Header;
        uint64_t Reserved;
    } __attribute__((packed));

    bool IsChecksumValid(const ACPI::SDTHeader *header);

    void *FindTable(const ACPI::SDTHeader *header, const char *signature);
}