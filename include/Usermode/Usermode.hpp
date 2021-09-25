#pragma once

#include <stdint.h>

#define USER_CODE_BASE 0x500000000
#define USER_DATA_BASE 0x600000000
#define USER_STACK_TOP 0x700000000

extern "C" void JumpToUserMode(void *syscallEntryAddress, void *userRSP, void *programAddress);