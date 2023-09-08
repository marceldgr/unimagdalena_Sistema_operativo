#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define col 10

int i = 0,j = 0,k = 0;
void matrizPar(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){
    int par = 0;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            if(par == 1){
                par = 0;
                for(k = 0;k<n;k++){
                    matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
                }
            }
            else{
                matrizC[i][j] = 0;
                par = 1;
            }
        }
    }
}

void matrizImpar(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){
    int impar = 1;
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            if(impar == 1){
                impar = 0;
                for(k = 0;k<n;k++){
                    matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
                }
            }
            else{
                matrizC[i][j] = 0;
                impar = 1;
            }
        }
    }
}

void diagonalSuperior(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            matrizC[i][j] = 0;
        }
    }
    for(i = 0;i<n;i++){
        for(j=n-1;j>i;j--){
            for(k = 0;k<n;k++){
                matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
            }
        }
    }
}

void diagonalInferior(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            matrizC[i][j] = 0;
        }
    }
    for(i = 1;i<n;i++){
        for(j=0;j<i;j++){
            for(k = 0;k<n;k++){
                matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
            }
        }
    }
}

void diagonalPrincipal(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){
    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            matrizC[i][j] = 0;
        }
    }
   for(i = 0;i<n;i++){
        for(j=0;j<n;j++){
            if(i == j){
                for(k = 0;k<n;k++){
                    matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
                }
            }
        }
    }
}

void llenarMatrices(int matrizA[][col],int matrizB[][col],int matrizC[][col],int n){

    for(i = 0;i<n;i++){
        for(j = 0;j<n;j++){
            matrizA[i][j] = rand()%5;
            matrizB[i][j] = rand()%5;
            matrizC[i][j] = 0;
        }
    }
}

void imprimirMatriz(int matriz[][col], int n){
    for(i = 0;i<n;i++){
        printf("\n");
        for(j = 0;j<n;j++){
            printf("%d ",matriz[i][j]);
        }
    }
    printf("\n");
}

int main(){
    srand(time(NULL));
    int n = 0;
    printf("Ingrese el tamano de la matriz: ");
    scanf("%d",&n);

    int matrizA[col][col];
    int matrizB[col][col];
    int matrizC[col][col];

    llenarMatrices(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizA,n);
    imprimirMatriz(matrizB,n);
    imprimirMatriz(matrizC,n);

    matrizImpar(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizC,n);

    matrizPar(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizC,n);

    diagonalSuperior(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizC,n);

    diagonalInferior(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizC,n);

    diagonalPrincipal(matrizA,matrizB,matrizC,n);

    imprimirMatriz(matrizC,n);

    system("pause");

    return 0;
}
