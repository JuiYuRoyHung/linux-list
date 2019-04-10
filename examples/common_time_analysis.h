#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif
#include <stdio.h>
#include <time.h>

double time_difference(struct timespec *start, struct timespec *end)
{
    struct timespec difference;

    if (end->tv_nsec < start->tv_nsec) {
        difference.tv_sec = end->tv_sec - start->tv_sec - 1;
        difference.tv_nsec = 1000000000 + end->tv_nsec - start->tv_nsec;
    } else {
        difference.tv_sec = end->tv_sec - start->tv_sec;
        difference.tv_nsec = end->tv_nsec - start->tv_nsec;
    }

    return (double) difference.tv_sec + difference.tv_nsec / 1000000000.0;
}
