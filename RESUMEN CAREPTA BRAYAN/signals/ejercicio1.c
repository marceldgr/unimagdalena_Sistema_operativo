#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

void manejador(){}

int main(int argc, char const *argv[])
{
    signal(SIGUSR1,manejador);
    pid_t childs[2], padre = getpid();
    int i;
    for ( i = 2; i >=0; i--) {
        if (!(childs[i] = fork())) break;
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
