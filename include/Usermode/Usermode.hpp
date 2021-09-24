#pragma once

#include <stdint.h>

extern "C" void JumpToUserMode(void *syscallEntryAddress, void *userRSP, void *programAddress);