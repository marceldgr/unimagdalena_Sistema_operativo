#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char const *argv[])
{
    int *turno;
    int *n;
    pid_t childs[3], padre = getpid();
    int nimp = 3;
   // printf("ingrese la cantidad de veces a imprimir:\n");
   // scanf("%d",&n);
    int shmId = shmget(IPC_PRIVATE, sizeof(int),IPC_CREAT | S_IRUSR | S_IWUSR);
    int shmId2 = shmget(IPC_PRIVATE, sizeof(int),IPC_CREAT | S_IRUSR | S_IWUSR);
    turno = shmat(shmId,NULL,0);
    n = shmat(shmId2,NULL,0);
    *turno = 0;
    *n = 0;
    int i;
    for( i = 0; i < 3; i++)
    {
        if(!(childs[i] = fork())) break;
    }
    
    if(getpid() == padre){
       
        //printf("%d ",i);
       // printf("estoy aqui");
        while(1){
            if(i == 3 && *turno == 0){
            printf("id: [%d] soy  padre \n",getpid());
            *turno = *turno+1;    
            if(*n > nimp) break;    
            }
        }
        for( int i = 0; i < 3; i++) wait(NULL);
        shmdt(n);
        shmdt(turno);
        shmctl(shmId,IPC_RMID,0);
        shmctl(shmId2,IPC_RMID,0);
    }else{
        sleep(3);
       while(1){
            if(i == 0 && *turno == 3){
                printf("id: [%d] soy  h%d \n",getpid(),i+1);
                *n = *n+1;
                *turno = 0;
                printf("el numero de impresiones es %d y nimp: %d\n",*n,nimp);
                printf("el turno es: %d \n",*turno);
                if(*n == nimp) break;
            }
            if(i == 1 && *turno == 2){
                printf("id: [%d] soy  h%d \n",getpid(),i+1);
                *turno = *turno+1;
            }
            
            if (i == 2 && *turno ==1) {
                printf("id: [%d] soy  h%d \n",getpid(),i+1);
                *turno = *turno+1;
            }
                
        }    
        shmdt(n);
        shmdt(turno);
    }

    return 0;
}
