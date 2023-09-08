#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(){
    system("clear");
    int np, n, i;
    int *turno;
    int shm_id;
    int shm_size = sizeof(int);

    shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
    turno = shmat(shm_id, 0, 0);
    *turno = 0;

    printf("Ingrese la cantidad de procesos hijos: ");
    scanf("%d", &np);
    printf("Ingrese la cantidad de veces a repetir: ");
    scanf("%d", &n);
    printf("\n\n");

    pid_t root = getpid();
    int miTurno = 0;

    for (i=0; i<np; i++){
        if (!fork()){
            miTurno = np - i;
            break;
        }
    }

    for (int j=0; j<n; j++){

        while(miTurno != *turno); //detienen al proceso en esta linea hasta que sea su turno

        if (miTurno == 0){
            printf("[%d] Soy el padre con turno %d\n", getpid(), miTurno);
        }else{
            printf("[%d] Soy el hijo %d con turno %d\n", getpid(), i+1, miTurno);
        }
        if (miTurno == np) *turno = 0;
        else *turno += 1;
    }
    shmdt(turno);
    if (getpid() == root) shmctl(shm_id, IPC_RMID, 0);

    sleep(1);
    if(root==getpid()){
        printf("\n\n");
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }

    return 0;
}
