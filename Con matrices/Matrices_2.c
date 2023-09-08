#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define col 20

typedef struct Datos{
    int n;
    int matrizA[col][col];
    int matrizB[col][col];
    int matrizC[col][col];
    int matrizD[col][col];
    int matrizE[col][col];
}Datos;

void manejador(){
}

void padre();
void hijo1();
void hijo2();

void error(const char *mensaje){
    printf("%s",mensaje);
    exit(1);
}

void llenarMatrices(Datos *datos){
    int i = 0,j = 0;
    for(i = 0;i<datos->n;i++){
        for(j = 0;j<datos->n;j++){
            datos->matrizA[i][j] = rand()%6;
            datos->matrizB[i][j] = rand()%6;
            datos->matrizC[i][j] = 0;
            datos->matrizD[i][j] = 0;
            datos->matrizE[i][j] = 0;
        }
    }
}

void imprimirMatriz(int matriz[][col],int n){
    int i = 0,j = 0;
    for(i = 0;i<n;i++){
        printf("\n");
        for(j = 0;j<n;j++){
            printf("%d ",matriz[i][j]);
        }
    }
}

void matrizImpar(Datos *datos){
    int i = 0,j = 0,k = 0;
    for(i = 0;i<datos->n;i++){
        for(j = i%2;j<datos->n;j+=2){
            for(k = 0;k<datos->n;k++){
                datos->matrizC[i][j] += datos->matrizA[i][k]*datos->matrizB[k][j];
                datos->matrizD[i][j] = datos->matrizC[i][j];
            }
        }
    }
}

void matrizPar(Datos *datos){
    int i = 0,j = 0,k = 0;
    for(i = 0;i<datos->n;i++){
        for(j = (i+1)%2;j<datos->n;j+=2){
            for(k = 0;k<datos->n;k++){
                datos->matrizC[i][j] += datos->matrizA[i][k]*datos->matrizB[k][j];
                datos->matrizE[i][j] = datos->matrizC[i][j];
            }
        }
    }
}

int main(){
    srand(time(NULL));
    signal(SIGUSR1,manejador);

    int shmid = shmget(IPC_PRIVATE,sizeof(Datos),IPC_CREAT|0666);

    if(shmid == -1){
        error("Error obteniendo la memoria");
    }
    void *shm_ptr = shmat(shmid,NULL,0);

    if(shm_ptr == (void*)-1){
        error("Error adjuntando el segmento");
    }

    int hijos[2], i = 0;

    Datos *datos = (Datos*) shm_ptr;
    datos->n = 0;

    for(i = 0;i<2;i++){
        if(!(hijos[i]=fork())){
            break;
        }
    }

    if(i == 0){
        pause();
        hijo1(datos,hijos);
        shmdt(shm_ptr);
    }
    else if(i == 1){
        pause();
        hijo2(datos,hijos);
        shmdt(shm_ptr);
    }
    else if(i == 2){
        usleep(1000);
        padre(datos,hijos);
        shmctl(shmid,IPC_RMID,NULL);
    }

    return 0;
}


void padre(Datos *datos,int hijos[]){
    int n = 0;
    printf("Ingrese el tamano de la matriz: ");
    scanf("%d",&n);
    datos->n = n;
    llenarMatrices(datos);

    kill(hijos[1],SIGUSR1);

    pause();

    printf("\nMatriz A:");
    imprimirMatriz(datos->matrizA,datos->n);
    printf("\nMatriz B:");
    imprimirMatriz(datos->matrizB,datos->n);
    printf("\nMatriz Impar:");
    imprimirMatriz(datos->matrizD,datos->n);
    printf("\nMatriz Par:");
    imprimirMatriz(datos->matrizE,datos->n);
    printf("\nMatriz Resultante");
    imprimirMatriz(datos->matrizC,datos->n);
}

void hijo1(Datos *datos,int hijos[]){
    matrizPar(datos);
    kill(getppid(),SIGUSR1);
}

void hijo2(Datos *datos,int hijos[]){
    matrizImpar(datos);
    kill(hijos[0],SIGUSR1);
}
