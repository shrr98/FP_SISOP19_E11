#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <pthread.h>
#include <vector>
#include <string.h>

char dir[] = "/home/trash/sisop/fp/FP_Sisop_E11/";
int resetFlag;

void resetAll(){

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
        resetFlag = 1;
        return 1;
    }
    return 0;
}

void *checkData(void *arg){
    
    while(1){
       if(modifiedChecker()){
           resetAll();
           startSchedule();
       }
        sleep(1);
    }
}

void* runSchedule(void *arg){       // argument passed here is a line in crontab.data
    int min, hour, daym, month, dayw;
    char program_path[512];
    char *line = (char*) arg;
    sprintf(line, "%d %d %d %d %d %s", &min, &hour, &daym, &month, &dayw, program_path);

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
    
    pthread_t th[2];
    int err;
    err = pthread_create(&th[0],NULL,&checkData,NULL);
    return 0;
}