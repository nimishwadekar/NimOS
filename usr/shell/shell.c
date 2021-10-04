#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>

int main()
{
    for(int i = 0; i < 100; i++)
    {
        sleepms(100);
        printf("%d\n", i);
    }

    while(1);
}