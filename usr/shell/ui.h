#ifndef _UI_H
#define _UI_H 1

#include <graphics.h>

#define SHELLTEXT_COLOUR        0x00FFFF00
#define TEXT_COLOUR             0x00F8F8F8

extern unsigned int res_width;
extern unsigned int res_height;

extern unsigned int foreground;
extern unsigned int background;

void printLogo(void);

void drawCursor(crsr_pos_t pos);
void eraseCursor(crsr_pos_t pos);

#endif