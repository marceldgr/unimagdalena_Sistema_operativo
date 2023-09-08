#include <stdio.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define col 20

void manejador(){
}

void hijo1();
void hijo2();
void hijo3();
void padre();

typedef struct Datos{

    int n;
    int matrizA[col][col];
    int matrizB[col][col];
    int matrizC[col][col];

}Datos;

void error(const char* mensaje){
    printf("%s\n",mensaje);
    exit(1);
}

void matrizSuperior(Datos *datos){
    int i = 0,j = 0,k = 0,n = datos->n;
    for(i = 0;i<n-1;i++){
        for(j = n-1;j>i;j--){
            for(k = 0;k<n;k++){
                datos->matrizC[i][j] += datos->matrizA[i][k]*datos->matrizB[k][j];
            }
        }
    }
}

void matrizPrincipal(Datos *datos){
    int i = 0,j = 0,k = 0;
    for(i = 0;i<datos->n;i++){
        for(k = 0;k<datos->n;k++){
            datos->matrizC[i][i] += datos->matrizA[i][k]*datos->matrizB[k][i];
        }
        printf("%d ",datos->matrizC[i][j]);
    }
}

void matrizInferior(Datos *datos){
    int i = 0,j = 0,k = 0,n = datos->n;
    for(i = 1;i<n;i++){
        for(j=0;j<i;j++){
            for(k = 0;k<n;k++){
                datos->matrizC[i][j] += datos->matrizA[i][k]*datos->matrizB[k][j];
            }
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
void llenarMatrices(Datos *datos){
    int i,j,n = datos->n;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            datos->matrizA[i][j] = rand()%6;
            datos->matrizB[i][j] = rand()%6;
            datos->matrizC[i][j] = 0;
        }
    }
}

int main(){

    srand(time(NULL));
    signal(SIGUSR1,manejador);

    int i = 0,n = 0;

    int shmid = shmget(IPC_PRIVATE,sizeof(Datos),IPC_CREAT|0666);

    if(shmid == -1){
        error("Error obteniendo el shmget");
    }

    void *shm_ptr = shmat(shmid,NULL,0);

    if(shm_ptr == (void*)-1){
        error("Error adjuntando el segmento");
    }

    Datos *datos = (Datos*) shm_ptr;
    datos->n = 0;

    int hijos[3];

    for(i = 0;i<3;i++){
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
        pause();
        hijo3(datos,hijos);
        shmdt(shm_ptr);
    }
    else if(i == 3){
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

        kill(hijos[2],SIGUSR1);
        pause();
        printf("\nMatriz A:");
        imprimirMatriz(datos->matrizA,datos->n);
        printf("\nMatriz B:");
        imprimirMatriz(datos->matrizB,datos->n);
        printf("\nMatriz C:");
        imprimirMatriz(datos->matrizC,datos->n);
}

void hijo1(Datos *datos, int hijos[]){
    matrizSuperior(datos);
    kill(getppid(),SIGUSR1);

}
void hijo2(Datos *datos,int hijos[]){

    matrizPrincipal(datos);
    kill(hijos[0],SIGUSR1);
}

void hijo3(Datos *datos,int hijos[]){

    matrizInferior(datos);
    kill(hijos[1],SIGUSR1);
}
