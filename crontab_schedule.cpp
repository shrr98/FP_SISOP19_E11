#include "crontab.h"

void timer(int sec){
    printf("%d sekon\n", sec);
    clock_t start = clock();
    int runtime = 0;
    while(!resetFlag && runtime < sec){
        runtime = (clock() - start) / CLOCKS_PER_SEC;
    }
    printf("ngesleep : %d\n", runtime);
    return;
}

void* runSchedule(void *arg){       // argument passed here is a line in crontab.data
    pthread_mutex_lock(&mutex1);
    int min, hour, daym, month, dayw;
    char m[3], h[3], d[3], mo[3], dw[2];
    char program_path[512];
    struct tm *now;
    bool isTheTime;                            // True if now is time scheduled
    char *line = (char*) arg;

    sscanf(line, "%s %s %s %s %s %s", m, h, d, mo, dw, program_path);

    printf("%s %s %s %s %s %s\n", m, h, d, dw, mo, program_path);
    pthread_mutex_unlock(&mutex1);
    min = m[0]=='*' ? -1 : atoi(m);
    hour = h[0]=='*' ? -1 : atoi(h);
    daym = d[0]=='*' ? -1 : atoi(d);
    dayw = dw[0]=='*' ? -1 : atoi(dw);
    month = mo[0]=='*' ? -1 : atoi(mo);

    // printf("%s\n line : %d %d %d %d %d %s\n", line, min, hour, daym, dayw, month, program_path);
    while(!resetFlag){
        printf("--- masuk while runschedule %s\n" , program_path);
        isTheTime = false;
        // cek dulu tapi masih magerr
        now = localtime(0);
        if(month==-1 || month == now->tm_mon){
            if(daym == -1 || daym == now->tm_mday){
                if(dayw==-1 || dayw == now->tm_wday){
                    if(hour==-1 || hour == now->tm_hour){
                        if(min==-1 || min == now->tm_min){
                            isTheTime = true;
                        }
                    }
                }
            }
        }
        // pid_t proses = fork();
        // printf("proses : %d\n", proses);
        // if(proses==0){
        //     if(!isTheTime){
        //             return 0;
        //     }
        //     else{
        //             printf("child : %s\n", program_path);
        //             char *args[] = {"bash", program_path, NULL};
        //             execv("/bin/bash", args);
        //     }
        // }
        // if(proses>0){
            // pthread_mutex_lock(&mutex1);
            // if(isTheTime)
            //     system(program_path);
                
            // pthread_mutex_unlock(&mutex1);
            printf("masuk parent process\n");
            int statp = 0;
            while(wait(&statp) > 0);
            timer(60);
            printf("--------childs mati kabeh\n");
        // }
    }
    return NULL;
}