#include <string.h>
#include <sys/syscall.h>
#include <time.h>

time_t time(time_t *time)
{
    time_t t = (unsigned long long) _syscall_0(SYS_TIME);
    if(time) *time = t;
    return t;
}

struct tm localtime(const time_t *time)
{
    struct tm t;
    t.tm_sec = (int) (*time & 0x3F);
    t.tm_min = (int) ((*time >> 6) & 0x3F);
    t.tm_hour = (int) ((*time >> 12) & 0x1F);
    t.tm_wday = (int) ((*time >> 17) & 0x07);
    t.tm_mday = (int) ((*time >> 20) & 0x1F);
    t.tm_mon = (int) ((*time >> 25) & 0x0F);
    t.tm_year = (int) ((*time >> 29) & 0xFFFFFFFF);
    return t;
}

time_t mktime(struct tm *timeptr)
{
    time_t t = (timeptr->tm_sec & 0x3F) | ((timeptr->tm_min & 0x3F) << 6) |
        ((timeptr->tm_hour & 0x1F) << 12) | ((timeptr->tm_wday & 0x07) << 17) |
        ((timeptr->tm_mday & 0x1F) << 20) | ((timeptr->tm_mon & 0x0F) << 25) |
        ((timeptr->tm_year & 0xFFFFFFFFl) << 29);
}

char *time_wday_str(time_t *time, char *buf, int len)
{
    static const char *wdays[8] = {NULL, "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    if(len == 0) return strcpy(buf, wdays[(*time >> 17) & 0x07]);
    memcpy(buf, wdays[(*time >> 17) & 0x07], len);
    buf[len] = 0;
    return buf;
}

char *time_mon_str(time_t *time, char *buf, int len)
{
    static const char *mons[13] = {NULL, "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    if(len == 0) return strcpy(buf, mons[(*time >> 25) & 0x0F]);
    memcpy(buf, mons[(*time >> 25) & 0x0F], len);
    buf[len] = 0;
    return buf;
}