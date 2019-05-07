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

void resetAll(std::vector<int> prosesList){
}

void *checkData(void *arg){
    FILE *pipe;
    char command[] = "find . -name \"crontab.data\" -cmin 0.66";    // check wherher crontab.data has been modifiend for last 1 second (approx 0.66 minutes)
    char buffer[64];
    while(1){
        memset(buffer, 0, sizeof(buffer));
        pipe = popen(command, "r");
        fread(buffer, sizeof(buffer), 1, pipe);
        if (buffer[0]!=0){
            resetFlag = 1;
        }
        sleep(1);
    }
}

std::vector<int> forking(){
    FILE *dataFile = fopen("crontab.data", "r");
    char buffer[512];
    while(fgets(buffer,512, dataFile)!=NULL){
        
    }
}

void *runPrograms(void *arg){
    std::vector<int> prosesList;
    prosesList = forking();
    while(1){
        if(!resetFlag){
            resetAll(prosesList);
        }
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
    
    pthread_t th[2];
    int err;
    err = pthread_create(&th[0],NULL,&checkData,NULL);
    err = pthread_create(&th[1], NULL, &runPrograms, NULL);
    return 0;
}