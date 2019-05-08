#include "crontab.h"

void resetAll(){
    resetFlag = 1;                                              // enable resetFlag so that the scheduling threads can be terminated
    for(int i=0; prosesThreads[i]!=NULLTHREAD; i++){
        pthread_join(prosesThreads[i], NULL);                   // waiting for all threads' termination
        prosesThreads[i] = NULLTHREAD;
    }
}

void startSchedule(){

}

int modifiedChecker(){
    FILE *pipe;
    char command[] = "find . -name \"crontab.data\" -cmin 0.66";    // check wherher crontab.data has been modifiend for last 1 second (approx 0.66 minutes)
    char buffer[64]; 
    memset(buffer, 0, sizeof(buffer));
    pipe = popen(command, "r");
    fread(buffer, sizeof(buffer), 1, pipe);
    if (buffer[0]!=0){
        return 1;
    }
    return 0;
}

void runCrontab(){
    while(1){
       if(modifiedChecker()){
           resetAll();
           startSchedule();
       }
        sleep(1);
    }
}


int main(int argc, char **argv){
    pid_t child_id;
    child_id = fork();
    if (child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if (child_id > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    if ((chdir(dir)) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    pid_t sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }
    
    runCrontab();
    return 0;
}