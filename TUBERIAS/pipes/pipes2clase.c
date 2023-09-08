#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>

int main(){
    pid_t hijo, padre = getpid();
    int fd1[2], fd2[2], i, n;
    char buffer[1024];

    if (pipe(fd1)) exit(-1);
    if (pipe(fd2)) exit(-1);

    for (i=0; i<3; i++){
        if (hijo = fork()) break;
    }

    if (getpid() != padre){
        if (i == 1){
            close(fd1[0]);
            close(fd2[0]);
            close(fd2[1]);
            strcpy(buffer, "Vengo del primer proceso hijo");
            write(fd1[1], buffer, strlen(buffer));
            close(fd1[1]);
        }
        if (i == 2){
            close(fd1[1]);
            close(fd2[0]);
            n = read(fd1[0], buffer, sizeof(buffer));
            buffer[n] = '\0';
            write(fd2[1], buffer, strlen(buffer));
            close(fd1[0]);
            close(fd2[1]);
        }
        if (i == 3){
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            n = read(fd2[0], buffer, sizeof(buffer));
            buffer[n] = '\0';
            printf("Process %d: Leido: %s \n", getpid(), buffer);
            close(fd2[0]);
        }
    }else{
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
    }

    if (padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d", getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    } else{
        sleep(3);
    }

    return 0;
}
