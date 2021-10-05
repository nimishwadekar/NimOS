#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>

int main()
{
    crsr_pos_t c = getcrsrpos();
    setcrsrpos(750, 16);
    printf("Date");
    setcrsrpos(c.x, c.y);
    if(lockscr(700, 0, 800, 50) == -1)
    {
        printf("Couldn't lock screen\n");
        while(1);
    }

    while(1) getchar();
    
    while(1);
}