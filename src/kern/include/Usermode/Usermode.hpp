#pragma once

#include <stdint.h>

#define USER_STACK_TOP 0x1000000

extern "C" void JumpToUserMode(void *syscallEntryAddress, void *userRSP, void *programAddress);