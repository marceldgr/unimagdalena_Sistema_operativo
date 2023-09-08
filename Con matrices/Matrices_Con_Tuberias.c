#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 4

typedef struct Datos{
    int x,y;
    int num;
}Datos;

int matrizA[N][N] = {
                    {1,2,3,4},
                    {-1,-2,-3,-4},
                    {2,0,1,0},
                    {3,0,1,2}
                    };

int matrizB[N][N] = {
                    {1,2,3,4},
                    {-1,-2,-3,-4},
                    {2,0,1,0},
                    {3,0,1,2}
                    };

int matrizC[N][N] = {
                    {0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0},
                    {0,0,0,0}
                    };

void matrizSuperior(Datos *vector){
    int i = 0,j = 0,k = 0,c = 0;
    for(i = 0;i<N-1;i++){
        for(j = N-1;j>i;j--){
            vector[c].num = 0;
            vector[c].x = j;
            vector[c].y = i;
            for(k = 0;k<N;k++){
                vector[c].num = vector[c].num + matrizA[i][k]*matrizB[k][j];
            }
            c++;
        }
    }
}

void matrizPrincipal(Datos *vector){
    int i = 0,j = 0,k = 0,c = 0;
    for(i = 0;i<N;i++){
        vector[c].num = 0;
        vector[c].x = i;
        vector[c].y = i;
        for(k = 0;k<N;k++){
            vector[c].num = vector[c].num + matrizA[i][k]*matrizB[k][i];
            vector[c].x = i;
            vector[c].y = i;
        }
        c++;
    }
}

void matrizInferior(Datos *vector){
    int i = 0,j = 0,k = 0,c = 0;
    for(i = 1;i<N;i++){
        for(j=0;j<i;j++){
            vector[c].num = 0;
            vector[c].x = j;
            vector[c].y = i;
            for(k = 0;k<N;k++){
                vector[c].num = vector[c].num + matrizA[i][k]*matrizB[k][j];
                vector[c].x = j;
                vector[c].y = i;
            }
            c++;
        }
    }
}

void escribirNumero(int *fd,int num){
    write(fd[1],&num,sizeof(int));
}

void escribirVector(int *fd,int n,Datos* datos){
    write(fd[1],datos,sizeof(Datos)*n);
}

int leerNumero(int *fd){
    int num;
    read(fd[0],&num,sizeof(int));
    return num;
}

void leerVector(int *fd,Datos *vector,int n){
    read(fd[0],vector,sizeof(Datos)*n);
}

void insertarEnMatriz(int *fd){
    int tamano = leerNumero(fd);
    Datos vector[tamano];
    leerVector(fd,vector,tamano);
    int i = 0;
    for(i = 0;i<tamano;i++){
    matrizC[vector[i].y][vector[i].x] = vector[i].num;
    }
}

void imprimirMatriz(int matriz[][N]){
    int i = 0,j = 0;
    for(i = 0;i<N;i++){
        printf("\n");
        for(j = 0;j<N;j++){
            printf("%d ",matriz[i][j]);
        }
    }
}
int main(){
    int tb21[2];
    int tb31[2];
    int tb41[2];

    pipe(tb21);

    if(fork()){
        pipe(tb31);
        if(fork()){
            pipe(tb41);
            if(fork()){
                printf("[%d] Soy el padre\n",getpid());
                close(tb21[1]);
                close(tb31[1]);
                close(tb41[1]);
                insertarEnMatriz(tb21);
                insertarEnMatriz(tb31);
                insertarEnMatriz(tb41);
                imprimirMatriz(matrizC);
            }else{
                printf("[%d] Soy el hijo 3\n",getpid());
                close(tb21[0]);
                close(tb21[1]);
                close(tb31[0]);
                Datos vector[N];
                matrizPrincipal(vector);
                escribirNumero(tb41,N);
                escribirVector(tb41,N,vector);
            }
        }else{
            printf("[%d] Soy el hijo 2\n",getpid());
            close(tb21[0]);
            close(tb21[1]);
            close(tb31[0]);
            int tamano = N*(N-1)/2;
            Datos vector[tamano];
            matrizSuperior(vector);
            escribirNumero(tb31,tamano);
            escribirVector(tb31,tamano,vector);
        }
    }else{
        printf("[%d] Soy el hijo 1\n",getpid());
        close(tb21[0]);
        int tamano = N*(N-1)/2;
        printf("[%d] tamano: %d \n",getpid(),tamano);
        Datos vector[tamano];
        printf("Se pudo crear esto\n");
        matrizInferior(vector);
        printf("Creo que se hizo esto\n");
        escribirNumero(tb21,tamano);
        escribirVector(tb21,tamano,vector);

    }
    return 0;
}
