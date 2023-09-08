#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main() {
    system("clear");
    int i, j, id, dato=0, fd[14][2];
    pid_t childs[3], root=getpid();

    for (int k=0; k<14; k++)
        if (pipe(fd[k])) exit(-1);

    id = 1;
    for (i=0; i<3; i++){
        childs[i] = fork();
        if (childs[i] == 0){
            id = (id*10)+i;

            int nhijos = (i != 1) ? 2:1;
            for (j=0; j<nhijos; j++){
                childs[0] = fork();
                if (childs[0] != 0) break;
                else id = (id*10)+j;
            }
            break;
        }
    }

    if (getpid() == root){
        char b[500];
        sprintf(b,"pstree -lp %d", getpid());
        system(b);
    } else sleep(3);

    if (id == 1){ // padre
        for (int a=0; a<14; a++){
            if (a != 13) close(fd[a][0]);
            if (a != 0) close(fd[a][1]);
        }

        printf("Proceso padre %d\n",getpid());
        write(fd[0][1], &dato, sizeof(int));

        read(fd[13][0], &dato, sizeof(int));
        printf("Proceso padre %d\n",getpid());
        // for (int k=0; k<3; k++) wait(NULL);

        close(fd[13][0]);   close(fd[0][1]);
    }else{ // procesos hijos

        if (i == 2){ // hijo 3 y sus decendientes
            if (id == 12){
                for (int a=0; a<14; a++){
                    if (a != 0 && a != 4) close(fd[a][0]);
                    if (a != 1 && a != 5) close(fd[a][1]);
                }

                read(fd[0][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[1][1], &dato, sizeof(int));

                read(fd[4][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[5][1], &dato, sizeof(int));

                close(fd[0][0]);    close(fd[1][1]);
                close(fd[4][0]);    close(fd[5][1]);
            }
            if (id == 120){
                for (int a=0; a<14; a++){
                    if (a != 1 && a != 3) close(fd[a][0]);
                    if (a != 2 && a != 4) close(fd[a][1]);
                }

                read(fd[1][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[2][1], &dato, sizeof(int));

                read(fd[3][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[4][1], &dato, sizeof(int));

                close(fd[1][0]);    close(fd[2][1]);
                close(fd[3][0]);    close(fd[4][1]);
            }
            if (id == 1201){
                for (int a=0; a<14; a++){
                    if (a != 2) close(fd[a][0]);
                    if (a != 3) close(fd[a][1]);
                }

                read(fd[2][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[3][1], &dato, sizeof(int));

                close(fd[2][0]);    close(fd[3][1]);
            }
        }
        if (i == 1){ // hijo 2 y sus decendientes
            if (id == 11){
                for (int a=0; a<14; a++){
                    if (a != 5 && a != 7) close(fd[a][0]);
                    if (a != 6 && a != 8) close(fd[a][1]);
                }

                read(fd[5][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[6][1], &dato, sizeof(int));

                read(fd[7][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[8][1], &dato, sizeof(int));

                close(fd[5][0]);    close(fd[6][1]);
                close(fd[7][0]);    close(fd[8][1]);
            }
            if (id == 110){
                for (int a=0; a<14; a++){
                    if (a != 6) close(fd[a][0]);
                    if (a != 7) close(fd[a][1]);
                }

                read(fd[6][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[7][1], &dato, sizeof(int));

                close(fd[6][0]);    close(fd[7][1]);
            }

        }
        if (i == 0){ // hijo 1 y sus decendientes
            if (id == 10){
                for (int a=0; a<14; a++){
                    if (a != 8 && a != 12) close(fd[a][0]);
                    if (a != 9 && a != 13) close(fd[a][1]);
                }

                read(fd[8][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[9][1], &dato, sizeof(int));

                read(fd[12][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[13][1], &dato, sizeof(int));

                close(fd[8][0]);    close(fd[9][1]);
                close(fd[12][0]);   close(fd[13][1]);
            }
            if (id == 100){
                for (int a=0; a<14; a++){
                    if (a != 9 && a != 11) close(fd[a][0]);
                    if (a != 10 && a != 12) close(fd[a][1]);
                }

                read(fd[9][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[10][1], &dato, sizeof(int));

                read(fd[11][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[12][1], &dato, sizeof(int));

                close(fd[9][0]);    close(fd[10][1]);
                close(fd[11][0]);   close(fd[12][1]);
            }
            if (id == 1001){
                for (int a=0; a<14; a++){
                    if (a != 10) close(fd[a][0]);
                    if (a != 11) close(fd[a][1]);
                }

                read(fd[10][0], &dato, sizeof(int));
                printf("Proceso %d [%d]\n", getpid(), getppid());
                write(fd[11][1], &dato, sizeof(int));

                close(fd[10][0]);    close(fd[11][1]);
            }
        }
    }

    return 0;
}
