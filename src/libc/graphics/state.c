#include <graphics.h>
#include <sys/syscall.h>

unsigned int getfg(void)
{
    return (unsigned int) _syscall_0(SYS_GETFORE);
}


int setfg(unsigned int argb)
{
    _syscall_1(SYS_SETFORE, argb);
    return 0;
}


unsigned int getbg(void)
{
    return (unsigned int) _syscall_0(SYS_GETBACK);
}


int setbg(unsigned int argb)
{
    _syscall_1(SYS_SETBACK, argb);
    return 0;
}

screen_res_t getscreenres(void)
{
    uint64_t res = (uint64_t) _syscall_0(SYS_GETRES);
    screen_res_t ret;
    ret.width = (unsigned int) (res >> 32);
    ret.height = (unsigned int) res;
    return ret;
}


crsr_pos_t getcrsrpos(void)
{
    uint64_t pos = (uint64_t) _syscall_0(SYS_GETCRSR);
    crsr_pos_t ret;
    ret.x = (unsigned int) (pos >> 32);
    ret.y = (unsigned int) pos;
    return ret;
}


int setcrsrpos(unsigned int x, unsigned int y)
{
    _syscall_2(SYS_SETCRSR, x, y);
    return 0;
}