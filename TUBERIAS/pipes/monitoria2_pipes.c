#include <stdio.h> // entrada y salida
#include <stdlib.h> // Funciones del sistema
#include <unistd.h> // fork()
#include <string.h>
#include <sys/types.h> // Macros de los tipos pid_t, size_t (typeof)

int main()
{
    pid_t childs[2], padre = getpid();
    int i, n;
    int fd1[2];
    int fd2[2];
    char buffer[1024];

    if (pipe(fd1)) exit(-1);
    if (pipe(fd2)) exit(-1);


    for (i=0; i<2; i++){
        if ( !(childs[i] = fork()) ) break;
    }

    if (getpid() != padre){
        if (i == 0 && childs[0] == 0){
            close(fd1[1]);
            close(fd2[0]);
            n = read(fd1[0], buffer, sizeof(buffer));
            buffer[n] = '\0';
            write(fd2[1], buffer, strlen(buffer));
            close(fd1[0]);
            close(fd2[1]);
        }
        if (i == 1 && childs[1] == 0){
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            n = read(fd2[0], buffer, sizeof(buffer));
            buffer[n] = '\0';
            printf("Process %d: leido -> %s\n", getpid(), buffer);
            close(fd2[0]);
        }
    }else{
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        strcpy(buffer, "Mensaje del padre");
        write(fd1[1], buffer, strlen(buffer));
        close(fd1[1]);
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
