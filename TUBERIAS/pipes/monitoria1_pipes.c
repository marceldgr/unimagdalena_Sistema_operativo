#include <stdio.h> // entrada y salida
#include <stdlib.h> // Funciones del sistema
#include <unistd.h> // fork()
#include <string.h>
#include <sys/types.h> // Macros de los tipos pid_t, size_t (typeof)

int main()
{
    pid_t hijo, padre = getpid();
    int i, n;
    int fd[2];
    char buffer[1024];

    if (pipe(fd) != 0){
        perror("Error: La tuberia no se creo!");
        exit(-1);
    }

    hijo = fork();

    if (getpid() != padre){
        if (hijo == 0){
            close(fd[1]);
            n = read(fd[0], buffer, sizeof(buffer));
            buffer[n] = '\0';
            printf("Process %d: leido -> %s\n", getpid(), buffer);
        }
    }else{
        close(fd[0]);
        strcpy(buffer, "Mensaje del padre");
        write(fd[1], buffer, strlen(buffer));
        close(fd[1]);
    }

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }

    return 0;
}
