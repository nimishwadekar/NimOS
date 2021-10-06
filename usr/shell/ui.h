#ifndef _UI_H
#define _UI_H 1

#include <graphics.h>

extern unsigned int res_width;
extern unsigned int res_height;

extern unsigned int foreground;
extern unsigned int background;

void printLogo(void);

void drawCursor(crsr_pos_t pos);
void eraseCursor(crsr_pos_t pos);

crsr_pos_t getCursorBack(char cur);

#endif