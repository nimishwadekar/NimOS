#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H 1

typedef struct _screen_res
{
    unsigned int width;
    unsigned int height;
} screen_res_t;

typedef struct _crsr_pos
{
    int x;
    int y;
} crsr_pos_t;

typedef int pid_t;

typedef unsigned char key_t;

#endif