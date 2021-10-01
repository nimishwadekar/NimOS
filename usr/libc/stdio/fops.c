#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

FILE *fopen(const char *pathname, const char *mode)
{
	int md;
    if(strcmp(mode, "r") == 0) md = 0;
    else if(strcmp(mode, "w") == 0) md = 1;
    else if(strcmp(mode, "r+") == 0) md = 2;
    else return NULL;

    int handle = _syscall_2(SYS_FOPEN, (int64_t) pathname, md);
    if(handle < 0) return NULL;

    FILE *file = malloc(sizeof(FILE));
    file->err = 0;
    file->handle = handle;
    file->pos = 0;

    _syscall_3(SYS_FSEEK, handle, 0, SEEK_END);
    file->length = _syscall_1(SYS_FTELL, handle);
    _syscall_3(SYS_FSEEK, handle, 0, SEEK_SET);
    file->eof = file->pos == file->length;

    return file;
}


int fclose(FILE *stream)
{
	int handle = stream->handle;
    memset(stream, 0, sizeof(FILE));
    free(stream);
    return _syscall_1(SYS_FCLOSE, handle);
}


size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	long read = _syscall_3(SYS_FREAD, stream->handle, (int64_t) ptr, size * nmemb);
    stream->pos += read;
    stream->eof = stream->pos == stream->length;
    return read;
}


int fseek(FILE *stream, long offset, int whence)
{
	stream->pos = _syscall_3(SYS_FSEEK, stream->handle, offset, whence);
    stream->eof = stream->pos == stream->length;
    return (int) stream->pos;
}


long ftell(FILE *stream)
{
	return _syscall_1(SYS_FTELL, stream->handle);
}