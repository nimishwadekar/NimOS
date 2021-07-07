#include "GDT.hpp"

GDT GlobalDescriptorTable
{
    {0, 0, 0, 0x00, 0x00, 0}, // Null
    {0, 0, 0, 0x9A, 0xA0, 0}, // Kernel Code
    {0, 0, 0, 0x92, 0xA0, 0}, // Kernel Data
    {0, 0, 0, 0xFA, 0xA0, 0}, // User Code
    {0, 0, 0, 0xF2, 0xA0, 0}, // User Data
};