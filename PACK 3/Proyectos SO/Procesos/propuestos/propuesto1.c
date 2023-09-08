#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main( void ) {
    
    pid_t root = getpid(); 
    int Nproc = 0;
    for(int i=0; i<3; i++){
        Nproc++;
        if( !fork() ){
            break;
        }
    }
    if( root == getpid() ){ 
        for(int j=0; j<3; j++)
        {
            printf("Nproc:%d\n",Nproc );
           if(Nproc % 2 != 0){
                fork();
                printf("Creando nieto %d ppid:%d Nproc:%d\n", getpid(), getppid(),Nproc);
            }
            wait(NULL);
        }
        printf("Finalizando padre %d\n", getpid());
    }
    else{


        printf("Finalizando hijo %d ppid:%d\n", getpid(), getppid());
       wait(NULL);
    }
    
    return EXIT_SUCCESS;
}