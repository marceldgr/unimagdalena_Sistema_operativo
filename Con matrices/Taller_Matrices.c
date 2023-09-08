#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int **matrizA,**matrizB,**matrizC;

typedef struct Datos{
    int n;
    unsigned long int id;
}Datos;

void imprimir_matriz(int **,int );
int **crear_matrices(int );
void llenar_matrices(int **, int);
void llenar_matriz3(int **,int);
void * calcular_matriz_diagonal(void *param);
void * calcular_matriz_superior(void *param);
void * calcular_matriz_inferior(void *param);
void borrar_matriz(int **, int);

int main(){
    srand(time(NULL));
    int n = 0,i = 0;
    printf("Ingrese el tamano de la matriz:");
    scanf("%d",&n);
    matrizA = crear_matrices(n);
    matrizB = crear_matrices(n);
    matrizC = crear_matrices(n);

    llenar_matrices(matrizA,n);
    llenar_matrices(matrizB,n);
    llenar_matriz3(matrizC,n);

    Datos *datos[3];
    datos[0] = (Datos*)calloc(n,sizeof(Datos));
    datos[1] = (Datos*)calloc(n,sizeof(Datos));
    datos[2] = (Datos*)calloc(n,sizeof(Datos));
    datos[0]->id = 0;
    datos[0]->n = n;
    datos[1]->id = 0;
    datos[1]->n = n;
    datos[2]->id = 0;
    datos[2]->n = n;
    pthread_t hilos[3];
    pthread_create(&hilos[0],NULL,calcular_matriz_inferior,datos[0]);
    pthread_create(&hilos[1],NULL,calcular_matriz_superior,datos[1]);
    pthread_create(&hilos[2],NULL,calcular_matriz_diagonal,datos[2]);

    for(i = 0;i<3;i++){
        pthread_join(hilos[i],NULL);
    }

    printf("\nMatriz A:");
    imprimir_matriz(matrizA,n);
    printf("\nMatriz B:");
    imprimir_matriz(matrizB,n);
    printf("\nMatriz C:");
    imprimir_matriz(matrizC,n);
    for(i = 0;i<3;i++){
        free(datos[i]);
    }

    borrar_matriz(matrizA,n);
    borrar_matriz(matrizB,n);
    borrar_matriz(matrizC,n);


    return 0;
}

void imprimir_matriz(int **m,int n){
    int i,j;
    for(i = 0;i<n;i++){
        printf("\n");
        for(j = 0;j<n;j++){
            printf("[%d]",m[i][j]);
        }
    }
    printf("\n");
}

int **crear_matrices(int n){
    int **m = (int **)calloc(n,sizeof(int*));
    int i;
    for(i = 0;i<n;i++){
        m[i] = (int *)calloc(n,sizeof(int));
    }
    return m;
}

void llenar_matrices(int **m,int n){
    int i,j;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            m[i][j] = rand()%10;
        }
    }
}

void llenar_matriz3(int **m,int n){
    int i,j;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            m[i][j] = 0;
        }
    }
}

void* calcular_matriz_inferior(void *param){
    Datos *datos = (Datos*)param;
    datos->id = pthread_self();
    printf("Hilo 1: [%lu] ejecutandose\n",datos->id);
    int i,j,k;
    int n = datos->n;
    for(i = 1;i<n;i++){
        for(j = 0;j<i;j++){
            for(k = 0;k<n;k++){
                matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
            }
        }
    }

}

void* calcular_matriz_superior(void * param){
    Datos *datos = (Datos*)param;
    datos->id = pthread_self();
    printf("Hilo 2: [%lu] ejecutandose\n",datos->id);
    int i,j,k;
    int n = datos->n;
    for(i = 0;i<n-1;i++){
        for(j = n-1;j>i;j--){
            for(k = 0;k<n;k++){
                matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
            }
        }
    }
}

void* calcular_matriz_diagonal(void *param){
    Datos *datos = (Datos*)param;
    datos->id = pthread_self();
    printf("Hilo 3: [%lu] ejecutandose\n",datos->id);
    int i,k;
    int n = datos->n;
    for(i = 0;i<n;i++){
        for(k = 0;k<n;k++){
            matrizC[i][i] += matrizA[i][k]*matrizB[k][i];
        }
    }
}


void borrar_matriz(int **m,int n){
    int i;
    for(i = 0;i<n;i++){
        free(m[i]);
    }
    free(m);
}
