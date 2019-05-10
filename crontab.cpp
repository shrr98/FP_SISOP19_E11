#include "crontab_schedule.cpp"

void resetAll(){
    printf("reset al\n");
    resetFlag = 1;                                              // enable resetFlag so that the scheduling threads can be terminated
    for(int i=0; prosesThreads[i]!=NULLTHREAD; i++){
        pthread_cancel(prosesThreads[i]);
        pthread_join(prosesThreads[i], NULL);                   // waiting for all threads' termination
        prosesThreads[i] = NULLTHREAD;
    }
}

void startSchedule(){
    char buffer[BUFSIZ];
    char filename[128];
    sprintf(filename, "%scrontab.data", dir);

    printf("MASUK START SCHEDULE\n%s\n", filename);
    FILE *crontabData = fopen(filename, "r");
    // FILE *dump = fopen("/home/trash/Documents/dump.txt", "w");
    int index = 0;
    while(fgets(buffer, BUFSIZ, crontabData)!=NULL){
        printf("%d. %s\n", index, buffer);
        // fprintf(dump, "%d. %s\n", index, buffer);
        pthread_create(&prosesThreads[index], NULL, runSchedule, (void*)buffer);
        index++;
        usleep(100);
    }
    // fclose(dump);
    fclose(crontabData);
}

int modifiedChecker(){
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

void runCrontab(){
    startSchedule();
    resetFlag = 0;
    while(1){
        printf("masuk while program utama\n");
       if(modifiedChecker()){
           resetAll();
           startSchedule();
       }
        sleep(1);
    }
}


int main(int argc, char **argv){
    // pid_t child_id;
    // child_id = fork();
    // if (child_id < 0) {
    //     exit(EXIT_FAILURE);
    // }

    // if (child_id > 0) {
    //     exit(EXIT_SUCCESS);
    // }

    // umask(0);

    // if ((chdir(dir)) < 0) {
    //     exit(EXIT_FAILURE);
    // }

    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    // pid_t sid = setsid();

    // if (sid < 0) {
    //     exit(EXIT_FAILURE);
    // }
    
    runCrontab();
    return 0;
}