#include <stdio.h>
#include "ui.h"

unsigned int res_width;
unsigned int res_height;

unsigned int foreground;
unsigned int background;

void printLogo(void)
{
    foreground = getfg();
    background = getbg();
    screen_res_t res = getscreenres();
    res_width = res.width;
    res_height = res.height;

    #define LOGO_LEN 7
    char logo[] = "TokyoOS";

    drawline(0, FONT_HEIGHT * 2, res_width, FONT_HEIGHT * 2, foreground);
    crsr_pos_t pos = getcrsrpos();
    setcrsrpos((res_width - 1 - LOGO_LEN * FONT_WIDTH) / 2, FONT_HEIGHT / 2);
    printf("%s", logo);
    setcrsrpos(pos.x, pos.y);
    printf("\n\n\n");
    lockscr(0, 0, res_width, 3 * FONT_HEIGHT);
    #undef LOGO_LEN
}