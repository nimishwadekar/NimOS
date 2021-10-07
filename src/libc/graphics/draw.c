#include <graphics.h>
#include <sys/syscall.h>

int drawpoint(unsigned int x, unsigned int y, unsigned int argb)
{
    _syscall_3(SYS_DRAWP, x, y, argb);
    return 0;
}


int drawline(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int argb)
{
    _syscall_5(SYS_DRAWL, x1, y1, x2, y2, argb);
    return 0;
}


int drawrect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int argb)
{
    _syscall_5(SYS_DRAWR, x, y, width, height, argb);
    return 0;
}


void drawCursor(crsr_pos_t pos)
{
    drawline(pos.x, pos.y + FONT_HEIGHT - 1, pos.x + FONT_WIDTH - 1, pos.y + FONT_HEIGHT - 1, getfg());
}


void eraseCursor(crsr_pos_t pos)
{
    drawline(pos.x, pos.y + FONT_HEIGHT - 1, pos.x + FONT_WIDTH - 1, pos.y + FONT_HEIGHT - 1, getbg());
}