#pragma once
#include <stdint.h>
#include "VFS.hpp"

// TEMPORARY FS TO VERIFY VFS

FILE fatopen(const char *filename);
void fatclose(FILE *file);
uint64_t fatread(FILE *file, void *buffer, const uint64_t length);
uint64_t fatwrite(FILE *file, const void *buffer, const uint64_t length);