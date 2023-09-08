#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int m = 0,p = 0,n = 0;
    srand(time(NULL));
    printf("Ingrese los valores de: \n");
    printf("M:"); scanf("%d",&m);
    printf("P:"); scanf("%d",&p);
    printf("N:"); scanf("%d",&n);

    int matrizA[m][p];
    int matrizB[p][n];
    int matrizC[m][n];

    int i = 0,j = 0;
    for(i = 0;i<m;i++){
        for(j = 0;j<p;j++){
            matrizA[i][j] = rand()%5;
        }
    }

    for(i = 0;i<p;i++){
        for(j = 0;j<n;j++){
            matrizB[i][j] = rand()%5;
        }
    }

    for(i = 0;i<m;i++){
        for(j = 0;j<n;j++){
            matrizC[i][j] = 0;
        }
    }

    int k = 0;
    for(i = 0;i<m;i++){
        for(j = 0;j<n;j++){
            for(k = 0;k<p;k++){
                matrizC[i][j] += matrizA[i][k]*matrizB[k][j];
            }
        }
    }

    printf("Matriz A:");
    for(i = 0;i<m;i++){
        printf("\n");
        for(j = 0;j<p;j++){
            printf("%d ",matrizA[i][j]);
        }
    }

    printf("\nMatriz B:");
    for(i = 0;i<p;i++){
        printf("\n");
        for(j = 0;j<n;j++){
            printf("%d ",matrizB[i][j]);
        }
    }

    printf("\nMatriz C:");
    for(i = 0;i<m;i++){
        printf("\n");
        for(j = 0;j<n;j++){
            printf("%d ",matrizC[i][j]);
        }
    }
    return 0;
}
