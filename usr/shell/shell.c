#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>

int main()
{
    crsr_pos_t c = getcrsrpos();
    setcrsrpos(750, 20);
    printf("Date");
    setcrsrpos(c.x, c.y);
    if(lockscr(700, 0, 800, 60) == -1)
    {
        printf("Couldn't lock screen\n");
        while(1);
    }

    drawline(50, 50, 750, 50, 0xFFFFFF);
    drawline(50, 150, 750, 150, 0xFFFFFF);

    while(1) getchar();
    
    while(1);
}