#ifndef _SYS_GRAPHICS_H
#define _SYS_GRAPHICS_H 1

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
int drawpoint(unsigned int x, unsigned int y, unsigned int argb);


/*
Draws line from (x1, y1) to (x2, y2) in colour argb.
*/
int drawline(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int argb);


/*
Draws rectangle at (x, y) of width and height in colour argb.
*/
int drawrect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int argb);

/*
Locks a rectangular portion of the screen with left diagonal ends at (x1,y1) and (x2,y2).
*/
int lockscr(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/*
Unlocks locked portion of screen.
*/
void unlockscr(void);


#endif