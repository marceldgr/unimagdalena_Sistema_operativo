#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

#define fitness(f1, f2) (f1*f2)/2

struct individuo{
    float f1, f2, fitness;
};

int main(){
    struct individuo *individuos;
    /*
        n: Numero de individuos
        k: Numero de procesos
        g: Numero de generaciones
    */
    int j,l,li, ls,i, n = 9, k = 3, g = 11, shmid_i, shmid_t, shmid_g, *turno, *num_gen; //g generaciones
    float aux;
    //Resevamos los segmentos de memoria
    shmid_i = shmget(IPC_PRIVATE, sizeof(struct individuo)*n, 0600);
    shmid_t = shmget(IPC_PRIVATE, sizeof(int), 0600);
    shmid_g = shmget(IPC_PRIVATE, sizeof(int), 0600);
    
    //Anexamos los segmentos compartidos a la tabla de direcciones
    individuos = shmat(shmid_i, NULL, 0);
    turno = shmat(shmid_t, NULL, 0);
    num_gen = shmat(shmid_g, NULL, 0);

    //Inicializacion de los segmentos compartidos (turno y num_gen)
    turno[0] = num_gen[0] = 0;

    //Creacion de los procesos
    for(i = 0; i < k; i++) if(!fork()) break;

    if(i != k){
        li = i * (n/k);
        ls = li + (n/k);
    }
    //#define fitness(f1, f2) (f1*f2)/2

    while(num_gen[0] < g){
        if(turno[0] == i)
            if(i == k){ //Proceso padre
                printf("\t\t***Generacion %d***\n", num_gen[0]+1);
                //Ordenamiento burbuja
                for(j = 0; j < n-1; j++)
                    for(l = j+1; l < n; l++){
                        if(individuos[l].fitness > individuos[j].fitness){
                            aux = individuos[j].fitness;
                            individuos[j].fitness = individuos[j].fitness;
                            individuos[l].fitness = aux;
                        }
                    }
                
                for(j = 0; j < (n/2); j++)
                    printf("%.2f\t", individuos[j].fitness);

                printf("\n\n");
                (*num_gen)++, (*turno)=0;
            }else{
                for(j = li; j < ls; j++){
                    individuos[j].f1 =  rand() % 100;
                    individuos[j].f2 =  rand() % 100;
                    individuos[j].fitness = fitness(individuos[j].f1, individuos[j].f2);
                }
                (*turno)++;
            }
    }

    return 0;
}


