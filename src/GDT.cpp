#include <GDT.hpp>

TSS TaskStateSegment {};

GDT GlobalDescriptorTable
{
    {0, 0, 0, 0x00, 0x00, 0}, // Null
    {0, 0, 0, 0x9A, 0xA0, 0}, // Kernel Code
    {0, 0, 0, 0x92, 0xA0, 0}, // Kernel Data
    {0, 0, 0, 0xFA, 0xA0, 0}, // User Code
    {0, 0, 0, 0xF2, 0xA0, 0}, // User Data
    {0, 0, 0, 0xFA, 0xA0, 0}, // User Code 2
    {(sizeof(TSS) & 0xFFFF), (uint16_t) ((uint64_t) &TaskStateSegment & 0xFFFF), (uint8_t)(((uint64_t) &TaskStateSegment >> 16) & 0xFF), 
        0b10001001, (0b01110000 | ((sizeof(TSS) >> 16) & 0xF)), (uint8_t)(((uint64_t) &TaskStateSegment >> 24) & 0xFF)} // Task State Segment
};