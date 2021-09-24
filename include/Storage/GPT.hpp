#pragma once

#include <stdint.h>

struct GPTHeader
{
    char Signature[8];
    uint32_t Revision;
    uint32_t HeaderSize;
    uint32_t CRC32Checksum;
    uint32_t Reserved0;
    
    uint64_t CurrentHeaderLBA;
    uint64_t AlternateHeaderLBA;
    uint64_t FirstUsableBlock;
    uint64_t LastUsableBlock;

    uint64_t DiskGUID[2];

    uint64_t EntriesStartLBA;
    uint32_t EntryCount;
    uint32_t EntrySize;
    uint32_t CRC32Entries;
} __attribute__((packed));

struct GUID
{
    uint32_t Data0;
    uint16_t Data1;
    uint16_t Data2;
    uint8_t Data3[8];
} __attribute__((packed));

struct GPTEntry
{
    GUID PartitionTypeGUID; // Zero implies unused entry.
    GUID UniquePartitionGUID;

    uint64_t StartingLBA;
    uint64_t EndingLBA;
    uint64_t Attributes;

    uint16_t Name[36]; // Unicode characters.
} __attribute__((packed));