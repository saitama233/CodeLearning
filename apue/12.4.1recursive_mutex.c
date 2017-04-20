#include "apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
//#include "12.2makethread.c"

extern int makethread(void *(*)(void *), void *);

struct to_info {
    void        (*to_fn)(void *);
    void        *to_arg;
    struct timespec to_wait;
};

#define SECTONSEC 1000000000

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000

void clock_gettime(int id, struct timespec *tsp)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    tsp->tv_sec = tv.tv_sec;
    tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void *timeout_helper(void *arg)
{
    printf("timeout_helper\n");
    struct to_info *tip;
    tip = (struct to_info *)arg;
    printf("tip->to_wait.tv_sec = %ld\n"
           "tip->to_wait.tv_nsec = %ld\n", 
           tip->to_wait.tv_sec, tip->to_wait.tv_nsec);
    clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return(0);
}

void timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
    printf("timeout\n");
    struct timespec now;
    struct to_info  *tip;
    int             err;

    clock_gettime(CLOCK_REALTIME, &now);
    printf("when->tv_sec = %ld\nwhen->tv_nsec = %ld\n", 
            when->tv_sec, when->tv_nsec);
    printf("now.tv_sec = %ld\nnow.tv_nsec = %ld\n", 
            now.tv_sec, now.tv_nsec);
    if ((when->tv_sec > now.tv_sec) ||
        (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
        tip = (struct to_info*)malloc(sizeof(struct to_info));
        if (tip != NULL) {
            tip->to_fn = func;
            tip->to_arg = arg;
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if (when->tv_nsec >= now.tv_nsec) {
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            } else {
                tip->to_wait.tv_sec--;
                tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + 
                    when->tv_nsec;
            }
            printf("tip->to_wait.tv_sec = %ld\n"
                   "tip->to_wait.tv_nsec = %ld\n", 
                    tip->to_wait.tv_sec, tip->to_wait.tv_nsec);
            err = makethread(timeout_helper, (void *)tip);
            if (err == 0)
                return;
            else
            {
                free(tip);
                printf("free(tip)\n");
            }
        }
    }
    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t     mutex;

void retry(void *arg)
{
    pthread_mutex_lock(&mutex);
    printf("5 seconds have passed\n");
    pthread_mutex_unlock(&mutex);
}

int main(void)
{
    int             err, condition, arg;
    struct timespec when;

    condition = 1;
    if ((err = pthread_mutexattr_init(&attr)) != 0)
        err_exit(err, "pthread_mutexattr_init failed");
    if ((err = pthread_mutexattr_settype(&attr, 
                    PTHREAD_MUTEX_RECURSIVE)) != 0)
        err_exit(err, "can't set recursive type");
    if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
        err_exit(err, "can't create recursive mutex");

    pthread_mutex_lock(&mutex);
    if (condition) {
        clock_gettime(CLOCK_REALTIME, &when);
        when.tv_sec += 5;
        timeout(&when, retry, (void *)((unsigned long)arg));
    }
    pthread_mutex_unlock(&mutex);
    sleep(10);
    printf("main end\n");

    exit(0);
}
