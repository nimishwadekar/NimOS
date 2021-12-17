#include <sys/shm.h>
#include <sys/syscall.h>

static void *_attached_shm = 0;

void *shmat(key_t key)
{
    void *shm = (void*) _syscall_1(SYS_SHMAT, key);
    if(shm) _attached_shm = shm;
    return shm;
}


int shmdt(void)
{
    _attached_shm = 0;
    return (int) _syscall_0(SYS_SHMDT);
}


void *shmget(void)
{
    return _attached_shm;
}