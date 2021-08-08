#pragma once
#include <stdint.h>

#define FS_INVALID 0
#define FS_VALID 1
#define FS_FILE (1 << 1)
#define FS_DIRECTORY (1 << 2)
#define FS_WRITE (1 << 3)
#define FS_APPEND (1 << 4)

// Including null terminator.
#define FILENAME_MAX_NULL 255
//#define DIRECTORY_MAX_NEST (FILENAME_MAX_NULL >> 1)
#define FILE_NAME_ERR -1

#define FILE_EOF -1

#define FILE_SEEK_SET 0
#define FILE_SEEK_CUR 1
#define FILE_SEEK_END 2

struct FILE
{
    char Name[FILENAME_MAX_NULL + 1];
    uint32_t ID;
    uint32_t Flags;
    uint8_t Device; // Volume, 'A' to 'Z'.
    uint64_t Length;
    uint64_t CurrentBlock;
    uint64_t Position; // Offset in file.
};

// Replaces directory separators with null character and stores in NameBuffer.
int ParseFileName(const char *fname, char *nameBuffer, uint8_t *dirIndices);