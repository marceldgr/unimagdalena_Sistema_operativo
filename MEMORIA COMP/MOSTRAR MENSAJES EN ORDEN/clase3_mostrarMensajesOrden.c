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
    int np=3, i, n=2;
    int *turno, *nvez;
    int shm_id, shm2_id;
    int shm_size = sizeof(int);

    shm_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
    shm2_id = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
    turno = shmat(shm_id, 0, 0);
    nvez = shmat(shm2_id, 0, 0);
    *turno = 0;
    *nvez = 0;

    pid_t root = getpid();

    for (i=0; i<np; i++) if (!fork()) break;

    if (getpid() != root){
        int salida = 0;
        while (1){
            if (*nvez == n){
                shmdt(turno);
                shmdt(nvez);
                salida++;
            }
            if (salida == 3) break;

            if (*turno == 3 && i == 0){
                printf("3Soy el Hijo %d [%d]\n", i+1, getpid());
                *turno = 0;
                *nvez += 1;
            }
            if (*turno == 2 && i == 1){
                printf("2Soy el Hijo %d [%d]\n", i+1, getpid());
                *turno += 1;
            }
            if (*turno == 1 && i == 2){
                printf("1Soy el Hijo %d [%d]\n", i+1, getpid());
                *turno += 1;
            }

        }

    }else{
        while (1){
            if (*turno == 0){
                if (*nvez == n){
                    shmdt(turno);
                    shmdt(nvez);
                    break;
                }else{
                    printf("Soy el Padre %d\n", getpid());
                    *turno += 1;
                }
            }
        }
    }

    if(root==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }

    return 0;
}
