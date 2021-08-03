#include "FAT.hpp"
#include "../Memory/Memory.hpp"
#include "../String.hpp"

uint8_t filebuffer[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

FILE f
{
    {"textfile.txt"},
    VFS_VALID | VFS_FILE | VFS_WRITE,
    'C',
    26,
    1,
    0
};

FILE fatopen(const char *filename)
{
    int len = strlen(filename);
    if(memcmp(f.Name, filename, len) == 0) return f;

    FILE file;
    file.Flags = VFS_INVALID;
    return file;
}

void fatclose(FILE *file)
{
    
}

uint64_t fatread(FILE *file, void *buffer, const uint64_t length)
{
    uint8_t *buf = (uint8_t*) buffer;
    for(uint64_t i = 0; i < length; i++) buf[i] = filebuffer[i];
    return length;
}

uint64_t fatwrite(FILE *file, const void *buffer, const uint64_t length)
{
    uint8_t *buf = (uint8_t*) buffer;
    for(uint64_t i = 0; i < length; i++) filebuffer[i] = buf[i];
    return length;
}