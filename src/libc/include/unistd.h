#ifndef	_UNISTD_H
#define	_UNISTD_H 1

#include <sys/types.h>

/*
DESCRIPTION
       getpid()  returns  the process ID (PID) of the calling process.  (This is often used by routines that generate
       unique temporary filenames.)

       getppid() returns the process ID of the parent of the calling process.  This will be  either  the  ID  of  the
       process  that  created  this  process  using fork(), or, if that process has already terminated, the ID of the
       process to which this process has been reparented (either init(1) or a "subreaper"  process  defined  via  the
       prctl(2) PR_SET_CHILD_SUBREAPER operation).
*/
pid_t getpid(void);
pid_t getppid(void);


int execv(const char *pathname, char *const argv[]);

// exitcode - Location to store exit code of spawned process in.
int spawnv(const char *pathname, char *const argv[], int *exitcode);

void exit(int status);


/* Make the process sleep for SECONDS seconds, or until a signal arrives
   and is not ignored.  The function returns the number of seconds less
   than SECONDS which it actually slept (thus zero if it slept the full time).
   If a signal handler does a `longjmp' or modifies the handling of the
   SIGALRM signal while inside `sleep' call, the handling of the SIGALRM
   signal afterwards is undefined.  There is no return value to indicate
   error, but if `sleep' returns SECONDS, it probably didn't work.
   This function is a cancellation point and therefore not marked with
   __THROW.  */
unsigned int sleep(unsigned int seconds);

double sleepms(double milliseconds);

#endif