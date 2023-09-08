#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


int main()
{
    pid_t padre = getpid();
    int i, j, n;
    char buffer[1024];

    printf("Ingrese el numero de n procesos:");
    scanf("%d", &n);

    pid_t childs[n];
    int fd[n][2];

    for (j=0; j<n; j++) if( pipe(fd[j]) ) exit(-1);
    for (i=0; i<n; i++) if(!(childs[i] = fork())) break;

    if (getpid() != padre){
        for (j=0; j<n; j++){
            if (i != j) close(fd[j][0]);
            if ((i+1) != j) close(fd[j][1]);
        }
        int tam;
        read(fd[i][0], &tam, sizeof(int));
        char lectura[tam];
        read(fd[i][0], lectura, tam);
        printf("Process %d: leido: %s\n", getpid(), lectura);
        write(fd[i+1][1], &tam, sizeof(int));
        write(fd[i+1][1], lectura, tam);
        close(fd[i][0]);
        close(fd[i+1][1]);
    }else{
        close(fd[0][0]);
        for (j=1; j<n; j++){
            close(fd[j][0]);
            close(fd[j][1]);
        }
        strcpy(buffer, "Mensaje del padre");
        int tam = strlen(buffer);
        write(fd[0][1], &tam, sizeof(int));
        write(fd[0][1], buffer, tam);
        close(fd[0][1]);
    }

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }

    return (0);
}
