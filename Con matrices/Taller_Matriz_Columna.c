#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int **mA,**mB,**mC;
int ** crear_matriz(int n);
void imprimir_matriz(int **m,int n);
void llenar_matriz(int **m,int n);
void llenar_matriz3(int **m,int n);
void *manejador(void *param);
void borrar_matriz(int **m,int n);

typedef struct Datos{
    int n;
    int j;
}Datos;

int main(){
    int n = 0,i;
    printf("Ingrese el tamano de la matriz: ");
    scanf("%d",&n);
    pthread_t hilos[n];
    Datos *datos[n];
    mA = crear_matriz(n);
    mB = crear_matriz(n);
    mC = crear_matriz(n);

    llenar_matriz(mA,n);
    llenar_matriz(mB,n);
    llenar_matriz3(mC,n);

    for(i = 0;i<n;i++){
        datos[i] = (Datos*)calloc(n,sizeof(Datos));
        datos[i]->j = i;
        datos[i]->n = n;
        pthread_create(&hilos[i],NULL,manejador,datos[i]);
    }

    for(i = 0;i<n;i++){
        pthread_join(hilos[i],NULL);
    }

    printf("\nMatriz A:");
    imprimir_matriz(mA,n);
    printf("\nMatriz B:");
    imprimir_matriz(mB,n);
    printf("\nMatriz C:");
    imprimir_matriz(mC,n);

    borrar_matriz(mA,n);
    borrar_matriz(mB,n);
    borrar_matriz(mC,n);

    for(i = 0;i<n;i++){
        free(datos[i]);
    }


    return 0;
}

int ** crear_matriz(int n){
    int ** m = (int**) calloc(n,sizeof(int*));
    int i;
    for(i = 0;i<n;i++){
        m[i] = (int *)calloc(n,sizeof(int));
    }
    return m;
}

void llenar_matriz(int **m,int n){
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

void * manejador(void *param){
    Datos *datos = (Datos*)param;
    int n = datos->n;
    int col = datos->j;
    int i,j;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            mC[i][col] += mA[i][j]*mB[j][col];
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
