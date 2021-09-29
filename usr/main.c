#include <stdio.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    const char *str = "1024";
    char buf[40] = {};
    /* _syscall_1(SYS_SETBACK, 0);
    _syscall_3(SYS_DRAWP, 50, 50, 0xFFFF);
    _syscall_5(SYS_DRAWL, 100, 200, 300, 400, 0xFFFF); */
    char *endptr;
    double ret = strtod("0.5", &endptr);
    int reti = strfromd(buf, 0, "%g", ret);
    if(*endptr == 0 && reti == 0) _syscall_1(SYS_PRINT, (int64_t) buf);
    else  _syscall_1(SYS_PRINT, (int64_t) "Error");

    return 0;
}