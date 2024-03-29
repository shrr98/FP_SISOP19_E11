#include "crontab.h"

void* runAJob(void *arg){
    int index = *(int*)(arg);
    struct cj *job = &cronJobs[index];
    time_t t = time(0);
    struct tm *now = localtime(&t);
    bool isTheTime = false;

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
        while(resetFlag);
        pthread_t thread[numJobs];
        int indlist[numJobs];
        for(int index=0; index<numJobs; index++){
            indlist[index] = index;
            pthread_create(&thread[index], NULL, runAJob, &indlist[index]);
        }
        sleep(60);
    }
    return NULL;
}

void resetAll(){
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

    FILE *crontabData = fopen(filename, "r");
    char m[3], h[3], d[3], mo[3], dw[2];
    char program_path[256];
    int index = 0;
    while(fscanf(crontabData, "%s %s %s %s %s", m, h, d, mo, dw)!=EOF){
        fgets(buffer, BUFSIZ, crontabData);
        cronJobs[index].min = m[0]=='*' ? -1 : atoi(m);
        cronJobs[index].hour = h[0]=='*' ? -1 : atoi(h);
        cronJobs[index].mday = d[0]=='*' ? -1 : atoi(d);
        cronJobs[index].wday = dw[0]=='*' ? -1 : atoi(dw);
        cronJobs[index].month = mo[0]=='*' ? -1 : atoi(mo)-1;
        strcpy(cronJobs[index].command, buffer);
        index++;
    }
    numJobs = index;
    fclose(crontabData);
    resetFlag = 0;
}

int modifiedChecker(){
    FILE *pipe;
    char command[] = "find . -name \"crontab.data\" -mmin -0.0167";    // check wherher crontab.data has been modifiend for last 1 second (approx 0.66 minutes)
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
    pthread_mutex_lock(&mutex1);
    startSchedule();
    pthread_mutex_unlock(&mutex1);
    while(1){
       if(modifiedChecker()&& !resetFlag){
            pthread_mutex_lock(&mutex1);
            resetAll();
            startSchedule();
            pthread_mutex_unlock(&mutex1);
       }
        sleep(1);
    }
}

