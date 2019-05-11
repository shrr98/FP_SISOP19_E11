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

#define MAXPROSES       1000
#define NULLTHREAD      0

char dir[] = "/home/trash/sisop/fp/FP_sisop_E11/";
int resetFlag;
pthread_t prosesThreads[MAXPROSES];
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void resetAll();                    
void startSchedule();
int modifiedChecker();
void runCrontab();
void* runSchedule(void *arg);

#endif