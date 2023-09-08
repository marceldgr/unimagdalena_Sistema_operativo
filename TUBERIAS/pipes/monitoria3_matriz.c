#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(){
    system("clear");
    pid_t root = getpid();
    int i, n;
    // float n;

    FILE *f = fopen("matriz.txt", "r");
    if (!f){
        printf("Error: No se puede abrir el archivo.\n");
        exit(-1);
    }
    fscanf(f, "%d", &n); //el primer numero es el n*n de la matriz y el numero de procesos

    int **fds = (int**)calloc(n, sizeof(int*)); //se define la matriz en memoria dinamica

    for (int a=0; a<n; a++) fds[a] = (int*)calloc(2, sizeof(int)); // se define cada una de la tuberia

    for (int a=0; a<n; a++) if (pipe(fds[a]) != 0) exit(-1); //creamos cada tuberia

    for (i=0; i<n; i++) if(!fork()) break; // creamos los procesos

    if(root == getpid()){
        // sleep(1);
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else sleep(3);

    if (getpid() == root){
        for (int a=0; a<n; a++){
            if (i != a) close(fds[a][0]);
            if (a != 0) close(fds[a][1]);
        }
        // close(fds[0][0]);
        // close(fds[1][0]);
        // close(fds[1][1]);
        // close(fds[2][0]);
        // close(fds[2][1]);

        float m[n][n];
        for (int a=0; a<n; a++){
            for (int b=0; b<n; b++){
                fscanf(f, "%f", &m[a][b]);
            }
        }
        fclose(f);

        for (int a=0; a<n; a++){
            for (int b=0; b<n; b++){
                write(fds[0][1], &m[a][b], sizeof(float));
            }
        }
        close(fds[0][1]);
        for (int a=0; a<n; a++) wait(NULL);

    }else{
        sleep(2);
        // printf("Ṕrocess %d i=%d\n", getpid(), i);
        for (int a=0; a<n; a++){
            if (i != a){
                printf("%d close(fds[%d][0])\n", i, a);
                close(fds[a][0]);
            }
            if ((a+1) != 0 && a != i+1){
                printf("%d close(fds[%d][1])\n", i, a);
                close(fds[a][1]);
            }
        }

        if (i == 0){


            float m[n][n];
            for (int a=0; a<n; a++){
                for (int b=0; b<n; b++){
                    read(fds[0][0], &m[a][b], sizeof(float));
                }
            }
            close(fds[0][0]);

            for (int a=0; a<n; a++){
                for (int b=0; b<n; b++) {
                    printf("M: %.2f\t", m[a][b]);
                }
                printf("\n");
            }

        }

    }




    return 0;
}
