#pragma once

#include <stdint.h>

extern "C" void JumpToUserMode(void *syscallEntry, void *stackTop, void *heapBase, void *programEntry);

extern "C" void JumpToUserAddress_Syscall(void *stackTop, void *heapBase, void *programEntry);