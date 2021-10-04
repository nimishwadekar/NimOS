#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>

int main()
{
    for(int y = 200; y < 250; y++)
    {
        sleepms(100);
        drawline(200, y, 600, y, 0xFFFF);
    }

    printf("\n-------------\n");
    while(1);
}
/**
 * 
 * REMOVE PRINTS FROM KERNEL.
 * 
 * REMOVE HANGS DUE TO ERRORS FROM KERNEL.
 * 
 * 
 */