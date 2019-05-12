#include "crontab.h"

void timer(int sec){
    // printf("%d sekon\n", sec);
    clock_t start = clock();
    int runtime = 0;
    while(!resetFlag && runtime < sec){
        runtime = (clock() - start) / CLOCKS_PER_SEC;
    }
    // printf("ngesleep : %d\n", runtime);
    return;
}

void* runAJob(void *arg){
    int index = *(int*)(arg);
    struct cj *job = &cronJobs[index];
    time_t t = time(0);
    struct tm *now = localtime(&t);
    bool isTheTime = false;
    printf("line : %d %d %d %d %d %s\n", now->tm_min, now->tm_hour, now->tm_mday, now->tm_wday, now->tm_mon, job->command);

    if(job->month==-1 || job->month == now->tm_mon){
        if(job->mday == -1 || job->mday == now->tm_mday){
            if(job->wday==-1 || job->wday == now->tm_wday){
                if(job->hour==-1 || job->hour == now->tm_hour){
                    if(job->min==-1 || job->min == now->tm_min){
                        isTheTime = true;
                    }
                }
            }
        }
    }
    if(isTheTime){
        system(job->command);
    }
}

void* runSchedule(void *arg){       // argument passed here is a line in crontab.data
    while(true){
        printf("---masuk while runSchedule\n");
        pthread_t thread[numJobs];
        int indlist[numJobs];
        for(int index=0; index<numJobs; index++){
            indlist[index] = index;
            pthread_create(&thread[index], NULL, runAJob, &indlist[index]);
            usleep(10);
        }
        timer(60);
    }
    return NULL;
}


void resetAll(){
    printf("reset all\n");
    pthread_mutex_lock(&mutex2);
    resetFlag = 1;                             // enable resetFlag so that the scheduling threads can be terminated
    for(int i=0; i<numJobs; i++){
        cronJobs[i] = NULLJOB;
    }              
    pthread_mutex_unlock(&mutex2);   
}

void startSchedule(){
    char buffer[256];
    char filename[128];
    sprintf(filename, "%scrontab.data", dir);

    printf("MASUK START SCHEDULE\n%s\n", filename);
    FILE *crontabData = fopen(filename, "r");
    char m[3], h[3], d[3], mo[3], dw[2];
    char program_path[256];
    int index = 0;
    ThreadArgs tharg;
    while(fscanf(crontabData, "%s %s %s %s %s", m, h, d, mo, dw)!=EOF){
        fgets(buffer, BUFSIZ, crontabData);
        cronJobs[index].min = m[0]=='*' ? -1 : atoi(m);
        cronJobs[index].hour = h[0]=='*' ? -1 : atoi(h);
        cronJobs[index].mday = d[0]=='*' ? -1 : atoi(d);
        cronJobs[index].wday = dw[0]=='*' ? -1 : atoi(dw);
        cronJobs[index].month = mo[0]=='*' ? -1 : atoi(mo)-1;
        strcpy(cronJobs[index].command, buffer);
        printf("%d. %s\n", index, buffer);
        index++;
    }
    numJobs = index;
    fclose(crontabData);
    resetFlag = 0;
}

int modifiedChecker(){
    // printf("masuk cek\n");
    FILE *pipe;
    char command[] = "find . -name \"crontab.data\" -mmin -0.167";    // check wherher crontab.data has been modifiend for last 1 second (approx 0.66 minutes)
    char buffer[64]; 
    memset(buffer, 0, sizeof(buffer));
    pipe = popen(command, "r");
    fread(buffer, sizeof(buffer), 1, pipe);
    pclose(pipe);
    if (buffer[0]!=0){
        return 1;
    }
    return 0;
}

void* runCrontab(void *arg){
    printf("masuk run\n");
    pthread_mutex_lock(&mutex1);
    startSchedule();
    pthread_mutex_unlock(&mutex1);
    while(1){
        // printf("masuk while program utama\n");
       if(modifiedChecker()&& !resetFlag){
            pthread_mutex_lock(&mutex1);
            resetAll();
            startSchedule();
            pthread_mutex_unlock(&mutex1);
       }
        timer(1);
    }
}

