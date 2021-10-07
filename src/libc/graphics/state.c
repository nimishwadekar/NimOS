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
    static screen_res_t ret = {0, 0};
    if(ret.width > 0) return ret;

    uint64_t res = (uint64_t) _syscall_0(SYS_GETRES);
    ret.width = (unsigned int) (res >> 32);
    ret.height = (unsigned int) res;
    return ret;
}


crsr_pos_t getcrsrpos(void)
{
    uint64_t pos = (uint64_t) _syscall_0(SYS_GETCRSR);
    crsr_pos_t ret;
    ret.x = (int) (pos >> 32);
    ret.y = (int) pos;
    return ret;
}


int setcrsrpos(unsigned int x, unsigned int y)
{
    _syscall_2(SYS_SETCRSR, x, y);
    return 0;
}


int scrscrolled(void)
{
    return (int) _syscall_0(SYS_SCRSCROLLED);
}


int lockscr(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
    return _syscall_4(SYS_LOCKSCR, x1, y1, x2, y2);
}


void unlockscr(void)
{
    _syscall_0(SYS_UNLOCKSCR);
}


crsr_pos_t moveCursorBack(crsr_pos_t pos)
{
    if(pos.x == 0 && pos.y == 0) return pos;
    pos.x -= FONT_WIDTH;
    if(pos.x < 0)
    {
        pos.y -= FONT_HEIGHT;
        pos.x = getscreenres().width - FONT_WIDTH;
    }
    return pos;
}