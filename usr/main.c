#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <unistd.h>
#include <time.h>

int main()
{
    time_t t = time(NULL);
    struct tm tim = localtime(&t);
    printf("%llu = %llu\n", t, mktime(&tim));

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