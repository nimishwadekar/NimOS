#ifndef _TIME_H
#define _TIME_H 1

struct tm
{
    int tm_sec;         /* seconds,  range 0 to 59                  */
    int tm_min;         /* minutes, range 0 to 59                   */
    int tm_hour;        /* hours, range 0 to 23                     */
    int tm_wday;        /* day of the week, range 1 to 7            */
    int tm_mday;        /* day of the month, range 1 to 31          */
    int tm_mon;         /* month, range 1 to 12                     */
    int tm_year;        /* year, range INT_MIN to INT_MAX           */
};

typedef unsigned long long time_t;

// Return current time. If time is not NULL, it is also stored at time.
time_t time(time_t *time);

struct tm localtime(const time_t *time);

time_t mktime(struct tm *timeptr);

// Full name if len = 0.
char *time_wday_str(time_t *time, char *buf, int len);

// Full name if len = 0.
char *time_mon_str(time_t *time, char *buf, int len);

#endif