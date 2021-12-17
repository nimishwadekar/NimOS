#pragma once

#define ARGV_BUFFER_ADDR    0x600000000
#define ARGV_BUFFER_SIZE    4000

extern char *ArgvBuffer;

void InitArgvBuffer();