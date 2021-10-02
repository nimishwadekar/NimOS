#pragma once

#include <stdint.h>

extern "C" void JumpToUserMode(void *syscallEntry, void *stackTop, void *heapBase, void *programEntry);