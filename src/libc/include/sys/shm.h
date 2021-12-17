#ifndef _SYS_SHM_H
#define _SYS_SHM_H 1

#include <sys/types.h>

// Attaches to shared memory segment with key "key".
void *shmat(key_t key);

// Detaches from attached memory segment. Returns 0 if successful or -1 if not attached.
int shmdt(void);

// Gets base address of currently attached shared memory segment. NULL if none.
void *shmget(void);

#endif