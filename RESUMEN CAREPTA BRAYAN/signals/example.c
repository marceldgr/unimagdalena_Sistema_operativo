#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

void manejador (int s){
    printf("signal No. [%d] proceso [%d]\n",s,getpid());
}
int main(int argc, char const *argv[])
{
    int c;
    signal(SIGUSR1,manejador);
    
    if ( !(c=fork()) ) {
        pause();
    }else{
        kill(c,SIGUSR1);
        wait(NULL);
    }
    
    return 0;
}
