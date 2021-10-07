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

    #define LOGO_LEN 7
    char logo[] = "TokyoOS";

    screen_res_t res = getscreenres();
    res_width = res.width;
    res_height = res.height;

    drawline(0, FONT_HEIGHT * 2, res.width, FONT_HEIGHT * 2, foreground);
    crsr_pos_t pos = getcrsrpos();
    setcrsrpos((res.width - 1 - LOGO_LEN * FONT_WIDTH) / 2, FONT_HEIGHT / 2);
    printf("%s", logo);
    setcrsrpos(pos.x, pos.y);
    printf("\n\n\n");
    lockscr(0, 0, res.width, 3 * FONT_HEIGHT);
    #undef LOGO_LEN
}