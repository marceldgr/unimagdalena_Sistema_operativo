#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void manejador(){}

int main() {
    signal(SIGUSR1,manejador);
    pid_t childs[2];
    int i;
    for ( i = 1; i >= 0 ; i--) {
        if (!(childs[i] = fork())) {
            break;
        }
    }

    if (i == -1) {
        sleep(1);
        printf("soy el padre\n");
        kill(childs[0],SIGUSR1);
        pause();
        printf("soy otra vez el padre\n");
        kill(childs[0],SIGUSR1);
    }else{
        if (i == 0) {
            pause();
            printf("soy el hijo 1\n");
            kill(childs[1],SIGUSR1);
            pause();
            printf("soy otra vez el hijo 1");
        }else{
            pause();
            printf("soy el hijo 2\n");
            kill(getppid(),SIGUSR1);
        }
    }
    return 0;
}
