#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>
#include "ui.h"

#define MAX_BUF_SIZE 255

char textbuf[MAX_BUF_SIZE + 1];

int main()
{
    printLogo();

    char c;
    char *textbufptr;
    crsr_pos_t crsrpos;
    while(1)
    {
        printf("shell> ");
        textbufptr = textbuf;
        while(1)
        {
            drawCursor(crsrpos = getcrsrpos());
            c = getchar();
            eraseCursor(crsrpos);
            
            if(c == '\n')
            {
                *textbufptr = 0;
                break;
            }

            if(c == '\b')
            {
                if(textbufptr == textbuf) setcrsrpos(crsrpos.x, crsrpos.y);
                else *(--textbufptr) = 0;
                continue;
            }
            
            if(textbufptr - textbuf >= MAX_BUF_SIZE)
            {
                putchar('\b');
                continue;
            }

            *(textbufptr++) = c;
        }

        if(textbufptr == textbuf) continue;

        printf("%s: command not found\n", textbuf);
    }
    
    while(1);
}