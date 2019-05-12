#include "crontab_schedule.cpp"

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
    pthread_create(&prosesThreads[0], NULL, runCrontab, NULL);
    pthread_join(prosesThreads[0], NULL);

    return 0;
}