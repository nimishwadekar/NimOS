#ifndef _SYS_DRAW_H
#define _SYS_DRAW_H 1

#include <sys/types.h>

/*
Get foreground colour (ARGB).
*/
unsigned int getfg(void);


/*
Set foreground colour (ARGB).
*/
int setfg(unsigned int argb);


/*
Get background colour (ARGB).
*/
unsigned int getbg(void);


/*
Set background colour (ARGB). Clears screen.
*/
int setbg(unsigned int argb);


/*
Get screen resolution.
*/
screen_res_t getscreenres(void);


/*
Get cursor position.
*/
crsr_pos_t getcrsrpos(void);


/*
Set cursor position.
*/
int setcrsrpos(unsigned int x, unsigned int y);

/*
Draws pixel at (x, y) in colour argb.
*/
int drawppint(unsigned int x, unsigned int y, unsigned int argb);


/*
Draws line from (x1, y1) to (x2, y2) in colour argb.
*/
int drawline(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int argb);


/*
Draws rectangle at (x, y) of width and height in colour argb.
*/
int drawrect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int argb);


/*
Draws circle at (x, y) of radius in colour argb.
*/
int drawcircle(unsigned int x, unsigned int y, unsigned int radius, unsigned int argb);

#endif