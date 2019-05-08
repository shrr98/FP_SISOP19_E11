#include "crontab.h"

void* runSchedule(void *arg){       // argument passed here is a line in crontab.data
    int min, hour, daym, month, dayw;
    char program_path[512];
    struct tm *now;
    bool isTheTime;                            // True if now is time scheduled
    char *line = (char*) arg;
    sprintf(line, "%d %d %d %d %d %s", &min, &hour, &daym, &month, &dayw, program_path);
    while(!resetFlag){
        isTheTime = false;
        // cek dulu tapi masih magerr
        now = localtime(0);
        if(month!='*' && month == now->tm_mon){
            if(daym != '*' && daym == now->tm_mday){
                if(dayw!='*' && dayw == now->tm_wday){
                    if(hour!='*' && hour == now->tm_hour){
                        if(min!='*' && min == now->tm_min){
                            isTheTime = true;
                        }
                        else if(min == '*'){
                            isTheTime = true;
                        }
                    }
                    else if(hour == '*'){
                        isTheTime = true;
                    }
                }
                else if(dayw == '*'){
                    isTheTime = true;
                }
            }
            else if(daym == '*'){
                isTheTime = true;
            }
        }
        else if(month == '*'){
            isTheTime = true;
        }

        if(isTheTime){
            // forking, terus jalanin program di child
            // sleep di parent nya
        }
    }
}