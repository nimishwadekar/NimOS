#pragma once

#include <stdint.h>

struct Process
{
    int ProcessID;
    uint32_t OpenFileHandles[8];
};