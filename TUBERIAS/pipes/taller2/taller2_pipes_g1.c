#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(){
    system("clear");
    int i, j, n, size;
    char *buffer, mensaje[1024];

    printf("Ingrese el numero de procesos hijos a crear: \n");
    scanf("%d", &n);

    pid_t root=getpid();
    int fd[n*2][2];

    for (j=0; j<n*2; j++) if( pipe(fd[j]) ) exit(-1);
    for (i=0; i<n; i++) if(fork()) break;

    if(root == getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else sleep(3);

    if (i == 0){ //padre
        close(fd[0][0]);
        close(fd[(n*2)-1][1]);
        for (int a=1; a<(n*2)-1; a++){
            close(fd[a][0]);
            close(fd[a][1]);
        }

        sleep(0.5);

        printf("Ingrese el mesaje a enviar por tuberias\n");
        scanf("%s", mensaje);

        int tam = strlen(mensaje);
        buffer = (char*)calloc(tam, sizeof(char));
        strcpy(buffer, mensaje);
        printf("Padre  [%d]: El mensaje: '%s' sera envido por tuberia\n", getpid(), buffer);

        write(fd[0][1], &tam, sizeof(int));
        write(fd[0][1], buffer, sizeof(char*)*tam);

        read(fd[(n*2)-1][0], &size, sizeof(int));
        read(fd[(n*2)-1][0], mensaje, sizeof(char)*size);
        printf("Padre  [%d]: Mesaje: %s\n", getpid(), mensaje);

        // for (int a=0; a<n; a++) wait(NULL);

        close(fd[0][1]);
        close(fd[(n*2)-1][0]);
    }else{
        if (i == 1){
            close(fd[0][1]);
            close(fd[(n*2)-1][0]);

            for (int a=1; a<(n*2)-1; a++){
                if (a != (n*2)-2) close(fd[a][0]);
                if (a != 1) close(fd[a][1]);
                // if (a != (n*2)-2) printf("close(fd[%d][0])\n", a);
                // if (a != 1) printf("close(fd[%d][1])\n", a);

            }

            read(fd[0][0], &size, sizeof(int));
            buffer = (char*)calloc(size, sizeof(char));
            read(fd[0][0], buffer, sizeof(char*)*size);
            printf("Hijo %d [%d]: Mesaje: %s\n", i, getpid(), buffer);

            write(fd[1][1], &size, sizeof(int));
            write(fd[1][1], buffer, sizeof(char*)*size);

            read(fd[(n*2)-2][0], &size, sizeof(int));
            buffer = (char*)calloc(size, sizeof(char));
            read(fd[(n*2)-2][0], buffer, sizeof(char*)*size);
            printf("Hijo %d [%d]: Mesaje: %s\n", i, getpid(), buffer);

            write(fd[(n*2)-1][1], &size, sizeof(int));
            write(fd[(n*2)-1][1], buffer, sizeof(char*)*size);


            close(fd[0][0]);
            close(fd[1][1]);
            close(fd[(n*2)-1][1]);
            close(fd[(n*2)-2][0]);
        }else{
            for (int a=0; a<(n*2); a++){
                if ((i-1) != a && a != ((n*2)-i)-1) close(fd[a][0]);
                if (a != i && a != (n*2)-i) close(fd[a][1]);
                // if ((i-1) != a && a != ((n*2)-i)-1) printf("close(fd[%d][0])\n", a);
                // if (a != i && a != (n*2)-i) printf("close(fd[%d][1])\n", a);
            }

            read(fd[i-1][0], &size, sizeof(int));
            buffer = (char*)calloc(size, sizeof(char));
            read(fd[i-1][0], buffer, sizeof(char*)*size);
            printf("Hijo %d [%d]: Mesaje: %s\n", i, getpid(), buffer);

            write(fd[i][1], &size, sizeof(int));
            write(fd[i][1], buffer, sizeof(char*)*size);

            if (i == n){
                write(fd[i+1][1], &size, sizeof(int));
                write(fd[i+1][1], buffer, sizeof(char*)*size);
            }

            if (i != n){
                read(fd[((n*2)-i)-1][0], &size, sizeof(int));
                buffer = (char*)calloc(size, sizeof(char));
                read(fd[((n*2)-i)-1][0], buffer, sizeof(char*)*size);
                printf("Hijo %d [%d]: Mesaje: %s\n", i, getpid(), buffer);

                write(fd[(n*2)-i][1], &size, sizeof(int));
                write(fd[(n*2)-i][1], buffer, sizeof(char*)*size);
            }

            close(fd[i-1][0]);
            close(fd[i][1]);
            close(fd[((n*2)-i)-1][0]);
            close(fd[(n*2)-i][1]);

        }
    }


    return 0;
}
