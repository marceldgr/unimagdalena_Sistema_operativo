#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void manejador(){}
int main() {
    signal(SIGUSR1,manejador);
    pid_t childs[3], padre = getpid();
    int i, nivel = 0;
    for ( i = 2; i >=0; i--) {
        if (!(childs[i] = fork())) {
            if(i == 0 || i == 2){
                if (!(childs[i] = fork())) {
                    break;
                }
            }
            break;
        }
    }

    if (i == -1) {
        sleep(1);
        printf("soy el padre\n");
        kill(childs[0],SIGUSR1);
        pause();
        printf("soy otra vez el padre\n");
    }else if(getppid() == padre){ //accedo al primer nivel
            if (i == 0) {
                pause();
                printf("soy el proceso 4\n");
                kill(childs[0],SIGUSR1);
                pause();
                printf("soy otra vez el proceso 4\n");
                kill(childs[1],SIGUSR1);
            }if (i == 1) {
                pause();
                printf("soy el proceso 5\n");
                kill(childs[2], SIGUSR1);
            }if (i == 2) {
                pause();
                printf("soy el proceso 6\n");
                kill(childs[2],SIGUSR1);
                pause();
                printf("soy otra vez el proceso 6\n");
                kill(getppid(),SIGUSR1);
            }
    }else{
        if(i ==0){
            pause();
            printf("soy el proceso 7 \n");
            kill(getppid(),SIGUSR1);
        }else{
            pause();
            printf("soy el proceso 8\n");
            kill(getppid(),SIGUSR1);
        }
    }

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(3);
}
    return 0;
}
