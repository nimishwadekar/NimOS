#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include "command.h"
#include "ui.h"

#define MAX_BUF_SIZE 255

char textbuf[MAX_BUF_SIZE + 1];

int main()
{
    printLogo();

    char c;
    char *textbufptr;
    char *cmdtok;
    crsr_pos_t crsrpos, crsrpos2;
    while(1)
    {
        setfg(SHELLTEXT_COLOUR);
        printf("shell> ");
        setfg(TEXT_COLOUR);
        textbufptr = textbuf;
        while(1)
        {
            drawCursor(crsrpos = getcrsrpos());
            c = getchar();
            if(scrscrolled()) crsrpos.y -= FONT_HEIGHT; // Erase cursor at correct position if screen has scrolled.
            eraseCursor(crsrpos);
            
            if(c == '\n')
            {
                *textbufptr = 0;
                break;
            }

            if(c == '\b')
            {
                if(textbufptr > textbuf)
                {
                    crsrpos = moveCursorBack(crsrpos);
                    crsrpos2 = crsrpos;
                    setcrsrpos(crsrpos.x, crsrpos.y);
                    putchar(' ');
                    setcrsrpos(crsrpos2.x, crsrpos2.y);
                    *(--textbufptr) = 0;
                }
                continue;
            }
            
            if(textbufptr - textbuf >= MAX_BUF_SIZE)
            {
                setcrsrpos(crsrpos.x, crsrpos.y);
                putchar(' ');
                setcrsrpos(crsrpos.x, crsrpos.y);
                continue;
            }

            *(textbufptr++) = c;
        }

        if(textbufptr == textbuf) continue;

        runCommand(textbuf);        
    }
    
    while(1);
}