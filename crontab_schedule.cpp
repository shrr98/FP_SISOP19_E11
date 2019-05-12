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
    ThreadArgs tharg = *(ThreadArgs*) arg;
    int index = tharg.index;
    char *line = tharg.line;
    pthread_mutex_lock(&mutex1[index]);
    int min, hour, daym, month, dayw;
    char m[3], h[3], d[3], mo[3], dw[2];
    char program_path[512];
    time_t current_time = time(NULL);
    struct tm *now;
    bool isTheTime;                            // True if now is time scheduled

    sscanf(line, "%s %s %s %s %s %s", m, h, d, mo, dw, program_path);

    printf("%s %s %s %s %s %s\n", m, h, d, dw, mo, program_path);
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
        now = localtime(&current_time);
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
    pthread_mutex_unlock(&mutex1[index]);

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


void resetAll(){
    printf("reset all\n");
    pthread_mutex_lock(&mutex2);
    resetFlag = 1;              
    pthread_mutex_unlock(&mutex2);                                // enable resetFlag so that the scheduling threads can be terminated
    for(int i=0; prosesThreads[i]!=NULLTHREAD; i++){
        pthread_cancel(prosesThreads[i]);
        pthread_join(prosesThreads[i], NULL);                   // waiting for all threads' termination
        prosesThreads[i] = NULLTHREAD;
    }
}

void startSchedule(){
    char buffer[BUFSIZ];
    char filename[128];
    char *tobearg;
    sprintf(filename, "%scrontab.data", dir);

    printf("MASUK START SCHEDULE\n%s\n", filename);
    FILE *crontabData = fopen(filename, "r");
    // FILE *dump = fopen("/home/trash/Documents/dump.txt", "w");
    resetFlag = 0;
    int index = 1;
    ThreadArgs tharg;
    while(fgets(buffer, BUFSIZ, crontabData)!=NULL){
        tobearg = (char*) malloc(128 * sizeof(char));
        strcpy(tobearg, buffer);
        printf("%d. %s\n", index, buffer);
        tharg.index = index;
        tharg.line = tobearg;
        // fprintf(dump, "%d. %s\n", index, buffer);
        pthread_create(&prosesThreads[index], NULL, runSchedule, (void*) &tharg);
        index++;
    }
    // fclose(dump);
    fclose(crontabData);
}

int modifiedChecker(){
    printf("masuk cek\n");
    FILE *pipe;
    char command[] = "find . -name \"crontab.data\" -cmin 0.66";    // check wherher crontab.data has been modifiend for last 1 second (approx 0.66 minutes)
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
    startSchedule();
    resetFlag = 0;
    while(1){
        printf("masuk while program utama\n");
       if(modifiedChecker()){
           resetAll();
           startSchedule();
       }
        timer(1);
    }
}

