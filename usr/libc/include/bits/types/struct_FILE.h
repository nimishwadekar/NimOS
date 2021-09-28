#ifndef _struct_FILE_defined
#define _struct_FILE_defined 1

struct _IO_FILE;

struct _IO_FILE
{
    int handle;
    int err;
};

#endif