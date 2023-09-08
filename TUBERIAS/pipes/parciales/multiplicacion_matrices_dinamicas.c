#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

void mostrarMatriz(int **m, int n, char *nombre){
    printf("***** Matriz %c *****\n", *nombre);
    for (int f=0; f<n; f++){
        for (int c=0; c<n; c++){
            printf("%d\t", m[f][c]);
        }
        printf("\n");
    }
    printf("\n");
}

int multiplicarMatriz(int **a, int **b, int n, int fil, int col){
    int resultado = 0;
    for (int x=0; x<n; x++){
        resultado += a[fil][x] * b[x][col];
    }
    return  resultado;
}

int main(){
    system("clear");
    int i, size, n;

    printf("Ingrese la dimencion de las matrices: ");
    scanf("%d", &n);

    if (n < 3){
        printf("Entrada incorrecta. La matriz no puede ser inferior a 3x3\n");
        exit(-1);
    }

    int **A = (int**)malloc(sizeof(int*)*n);
    int **B = (int**)malloc(sizeof(int*)*n);
    int **C = (int**)malloc(sizeof(int*)*n);
    for (int a=0; a<n; a++){
        A[a] = (int*)malloc(sizeof(int)*n);
        B[a] = (int*)malloc(sizeof(int)*n);
        C[a] = (int*)malloc(sizeof(int)*n);
    }

    int fd[4][2]; // usaremos 4 tuberias

    for (int j=0; j<4; j++)
        if ( pipe(fd[j]) ) exit(-1);

    for (i=0; i<3; i++) if(!fork()) break;

    if (i == 3){
        char b[500];
        sprintf(b,"pstree -lp %d", getpid());
        system(b);
    } else sleep(3);

    if (i == 3) { // si es el proceso padre

        for (int a=0; a<=3; a++){
            if (a != 3) close(fd[a][0]);
            if (a != 0) close(fd[a][1]);
        }

        int cont = 1;
        for (int f=0; f<n; f++)
            for (int c=0; c<n; c++)
                A[f][c] = cont++;

        cont = n*n;
        for (int f=0; f<n; f++)
            for (int c=0; c<n; c++)
                B[f][c] = cont--;

        for (int f=0; f<n; f++)
            for (int c=0; c<n; c++)
                C[f][c] = 0;

        printf("\n[%d] Padre: Estas son las matrices\n", getpid());
        mostrarMatriz(A, n, "A");
        mostrarMatriz(B, n, "B");
        mostrarMatriz(C, n, "C");

        for (int f=0; f<n; f++){
            for (int c=0; c<n; c++){
                write(fd[0][1], &A[f][c], sizeof(int));
                write(fd[0][1], &B[f][c], sizeof(int));
                write(fd[0][1], &C[f][c], sizeof(int));
            }
        }

        for (int f=0; f<n; f++){
            for (int c=0; c<n; c++){
                read(fd[i][0], &A[f][c], sizeof(int));
                read(fd[i][0], &B[f][c], sizeof(int));
                read(fd[i][0], &C[f][c], sizeof(int));
            }
        }
        printf("\n[%d] Padre: Estas es la matriz resultante\n", getpid());
        mostrarMatriz(C, n, "C");

        // for (int z=0; z<3; z++) wait(NULL);

        close(fd[0][1]);
        close(fd[3][0]);
    }else{ // si son procesos hijos
        for (int a=0; a<=3; a++){
            if (a != i) close(fd[a][0]);
            if (a != (i+1)) close(fd[a][1]);
        }

        for (int f=0; f<n; f++){
            for (int c=0; c<n; c++){
                read(fd[i][0], &A[f][c], sizeof(int));
                read(fd[i][0], &B[f][c], sizeof(int));
                read(fd[i][0], &C[f][c], sizeof(int));
            }

        }

        if (i == 0){
            // elementos bajo la diagonal
            for (int f=1; f<n; f++)
                for (int c=0; c<f; c++)
                    C[f][c] = multiplicarMatriz(A, B, n, f, c);
        }
        if (i == 1){
            // elementos subre la diagonal
            for (int f=n-2; f>=0; f--)
                for (int c=n-1; c>f; c--)
                    C[f][c] = multiplicarMatriz(A, B, n, f, c);
        }
        if (i == 2){
            // elementos en la diagonal
            for (int f=0; f<n; f++)
                for (int c=0; c<n; c++)
                    if(f == c) C[f][c] = multiplicarMatriz(A, B, n, f, c);
        }

        printf("[%d] Hijo %d: Esta es la matriz resultado\n", getpid(), i+1);
        mostrarMatriz(C, n, "C");

        for (int f=0; f<n; f++){
            for (int c=0; c<n; c++){
                 write(fd[i+1][1], &A[f][c], sizeof(int));
                 write(fd[i+1][1], &B[f][c], sizeof(int));
                 write(fd[i+1][1], &C[f][c], sizeof(int));
            }
        }


        close(fd[i][0]);
        close(fd[i+1][1]);
    }


    return 0;
}
