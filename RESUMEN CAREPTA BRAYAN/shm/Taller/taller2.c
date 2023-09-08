#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 100
//#define sizeElement 10000000

int main(){
    int *turno;
    
    int turnoId;
    int shm_size = MAX_SIZE*sizeof(double);
    turnoId = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
    turno = shmat(turnoId,0,0);
    *turno = 0;
    
    int nHijos; // cantidad de hijos
    printf("ingrese el numero de hijos: ");
    scanf("%d",&nHijos);
    int n; // ne veces que se va a repetir
    printf("ingrese el numero de veces a imprimir: ");
    scanf("%d",&n);
    int mTurno = 0;
    pid_t padre = getpid(), childs[nHijos];
    int i;
    for(i = 0; i < nHijos; i++){
        childs[i] = fork();
        if(!childs[i]) {
            mTurno = nHijos-i;
            break;
        }
    }

    for(int j = 0; j < n; j++){
        while(mTurno != *turno);
        if(mTurno == 0 && i == nHijos){
            printf("<soy el padre  con el turno %d>\n",mTurno);
        }else{
             printf("soy el hijo %d con el turno %d\n",i+1,mTurno);
        }
        *turno = *turno+1;
        if(mTurno == nHijos){
            *turno=0;
        }
    }
    shmdt(turno);
    if(padre == getpid()){
        shmctl(turnoId, IPC_RMID, 0);
    }
return 0;
}