#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

void mostrarMatriz(int m[4][4], char *nombre){
    printf("***** Matriz %c *****\n", *nombre);
    for (int f=0; f<4; f++){
        for (int c=0; c<4; c++){
            printf("%d\t", m[f][c]);
        }
        printf("\n");
    }
    printf("\n");
}

int multiplicarMatriz(int a[4][4], int b[4][4], int fil, int col){
    int resultado = 0;
    for (int x=0; x<4; x++){
        resultado += a[fil][x] * b[x][col];
    }
    return  resultado;
}

int main(){
    system("clear");
    int n=4;

    int i, A[n][n], B[n][n], C[n][n];
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

        cont = 1;
        for (int f=0; f<n; f++)
            for (int c=0; c<n; c++)
                B[f][c] = cont++;

        for (int f=0; f<n; f++)
            for (int c=0; c<n; c++)
                C[f][c] = 0;

        printf("\n[%d] Padre: Estas son las matrices\n", getpid());
        mostrarMatriz(A, "A");
        mostrarMatriz(B, "B");
        mostrarMatriz(C, "C");

        write(fd[0][1], A, sizeof(int)*(n*n));
        write(fd[0][1], B, sizeof(int)*(n*n));
        write(fd[0][1], C, sizeof(int)*(n*n));

        read(fd[i][0], A, sizeof(int)*(n*n));
        read(fd[i][0], B, sizeof(int)*(n*n));
        read(fd[i][0], C, sizeof(int)*(n*n));
        printf("\n[%d] Padre: Estas es la matriz resultante\n", getpid());
        mostrarMatriz(C, "C");

        close(fd[0][1]);
        close(fd[3][0]);
    }else{ // si son procesos hijos
        for (int a=0; a<=3; a++){
            if (a != i) close(fd[a][0]);
            if (a != (i+1)) close(fd[a][1]);
        }

        read(fd[i][0], A, sizeof(int)*(4*4));
        read(fd[i][0], B, sizeof(int)*(4*4));
        read(fd[i][0], C, sizeof(int)*(4*4));
        // mostrarMatriz(A, "A");
        // mostrarMatriz(B, "B");

        if (i == 0){
            // elementos bajo la diagonal
            for (int f=1; f<4; f++)
                for (int c=0; c<f; c++)
                    C[f][c] = multiplicarMatriz(A, B, f, c);
        }
        if (i == 1){
            // elementos subre la diagonal
            for (int f=2; f>=0; f--)
                for (int c=3; c>f; c--)
                    C[f][c] = multiplicarMatriz(A, B, f, c);
        }
        if (i == 2){
            // elementos en la diagonal
            for (int f=0; f<4; f++)
                for (int c=0; c<4; c++)
                    if(f == c) C[f][c] = multiplicarMatriz(A, B, f, c);
        }

        printf("[%d] Hijo %d: Esta es la matriz resultado\n", getpid(), i+1);
        mostrarMatriz(C, "C");

        write(fd[i+1][1], A, sizeof(int)*(4*4));
        write(fd[i+1][1], B, sizeof(int)*(4*4));
        write(fd[i+1][1], C, sizeof(int)*(4*4));

        close(fd[i][0]);
        close(fd[i+1][1]);
    }


    return 0;
}
