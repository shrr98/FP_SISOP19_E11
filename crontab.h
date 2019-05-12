#ifndef CRONTAB_H
#define CRONTAB_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <pthread.h>
#include <vector>
#include <string.h>
#include <time.h>

#define MAXPROSES       1000
#define NULLTHREAD      0

typedef struct {
    int index;
    char *line;
} ThreadArgs;

struct cj{
    int min, hour, mday, wday, month;
    char command[256];
} cronJobs[MAXPROSES];

struct cj NULLJOB = {
    .min = -2,
    .hour = -2,
    .mday = -2,
    .wday = -2,
    .month = -2,
};

int numJobs;

char dir[] = "/home/trash/sisop/fp/FP_sisop_E11/";
int resetFlag;
pthread_t prosesThreads[2];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void resetAll();                    
void startSchedule();
int modifiedChecker();
void* runCrontab(void* arg);
void* runSchedule(void *arg);
void* runAJob(void *arg);

#endif