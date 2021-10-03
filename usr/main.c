#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>

int main()
{
    for(int y = 200; y < 300; y++) drawline(200, y, 600, y, 0xFFFF);

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