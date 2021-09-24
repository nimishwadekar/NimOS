#include <FS/File.hpp>
#include <Memory/Memory.hpp>
#include <String.hpp>

int ParseFileName(const char *fname, char *nameBuffer, uint8_t *dirIndices)
{
    int len = strlen(fname);
    if(len > FILENAME_MAX_NULL) return FILE_NAME_ERR;

    memcpy(fname, nameBuffer, len);
    for(int i = 0, diri = 1; i < len; i++)
    {
        if(nameBuffer[i] == '/') 
        {
            nameBuffer[i] = 0;
            dirIndices[diri] = i + 1;
            diri += 1;
        }
    }
    return len;
}