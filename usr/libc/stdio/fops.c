#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>

FILE _openfiles[8] = {};
int _opencount = 0;

FILE *fopen(const char *pathname, const char *mode)
{
	int md;
    if(strcmp(mode, "r") == 0) md = 0;
    else if(strcmp(mode, "w") == 0) md = 1;
    else if(strcmp(mode, "r+") == 0) md = 2;
    else return NULL;


}


int fclose(FILE *stream)
{
	
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	
}


int fseek(FILE *stream, long offset, int whence)
{
	
}


long ftell(FILE *stream)
{
	
}