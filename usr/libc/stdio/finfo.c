#include <stdio.h>

int feof(FILE *stream)
{
	return stream->eof;
}


int ferror(FILE *stream)
{
	return stream->err;
}


int fileno(FILE *stream)
{
	return stream->handle;
}


void clearerr(FILE *stream)
{
	stream->err = 0;
}