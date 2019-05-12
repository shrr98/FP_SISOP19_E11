#include <pthread.h>
#include <stdio.h>

void* a(void* arf){
    printf("masuk thread\n");
}

int main(){
    pthread_t t;
    printf("aaaaaaa");
    pthread_create(&t, NULL, a, NULL);
    printf("bbb");
    return 0;
}