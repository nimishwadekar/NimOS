#include <Environment.hpp>
#include <Memory/Memory.hpp>
#include <String.hpp>

Env Environment;

#define KernelKey       "kernel"
#define KernelKeyLen    6
#define ScreenKey       "screen"
#define ScreenKeyLen    6
#define TimezoneKey     "timezone"
#define TimezoneKeyLen  8

// Pair of type "key=value".
static unsigned char *GetValue(unsigned char *keyValuePair)
{
    for( ; *keyValuePair; keyValuePair++)
        if(*keyValuePair == '=') return keyValuePair + 1;
    return nullptr;
}

void Env::ParseEnvironemnt(unsigned char *environment)
{
    Kernel = Screen = Timezone = nullptr;

    bool kernelFlag = true, screenFlag = true, timezoneFlag = true;
    unsigned char *tok = strtok(environment, "\n");
    while(tok)
    {
        if(kernelFlag && memcmp(tok, KernelKey, KernelKeyLen) == 0)
        {
            unsigned char *value = GetValue(tok);
            if(value) Kernel = value;
            kernelFlag = false;
        }
        else if(screenFlag && memcmp(tok, ScreenKey, ScreenKeyLen) == 0)
        {
            unsigned char *value = GetValue(tok);
            if(value) Screen = value;
            screenFlag = false;
        }
        else if(timezoneFlag && memcmp(tok, TimezoneKey, TimezoneKeyLen) == 0)
        {
            unsigned char *value = GetValue(tok);
            if(value) Timezone = value;
            timezoneFlag = false;
        }
        tok = strtok(nullptr, "\n");
    }
}