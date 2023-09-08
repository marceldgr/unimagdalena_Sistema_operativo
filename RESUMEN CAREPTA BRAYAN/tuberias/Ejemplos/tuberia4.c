#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
   int i, j = -1, n;
    pid_t childs[3], padre = getpid();
    int fd1[2];
    int fd2[2];
    int fd3[2];
    int fd4[2];
    int fd5[2];
    int fd6[2];
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    pipe(fd5);
    pipe(fd6);
    char buffer[1024];

    for ( i = 0; i < 3; i++) {
        if (!(childs[i] = fork())) {
            if( i == 2){
                for ( j = 0; j < 2; j++) {
                    if (childs[j] = fork()) {
                        break;
                    }
                }
            }
        break;
        }
    }

    if (getpid() != padre) {
        
        if (i == 0 && childs[0] == 0) {
        close(fd1[1]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        n = read(fd1[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        write(fd2[1],buffer,strlen(buffer));
        close(fd1[0]);
        close(fd2[1]);
        }

        if (i == 1 && childs[1] ==0) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        n = read(fd2[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        write(fd3[1],buffer,strlen(buffer));
        close(fd2[0]);
        close(fd3[1]);
        }
        
        if (i == 2 && j == 0) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        n = read(fd3[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        write(fd4[1],buffer,strlen(buffer));
        close(fd3[0]);
        close(fd4[1]);
        }     
        
        if (i == 2 && j == 1) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd6[0]);
        close(fd6[1]); 
        n = read(fd4[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        //printf("%s  estoy en %d cuando i:%d y j:%d ",buffer,getpid(),i,j);
        write(fd5[1],buffer,strlen(buffer));
        close(fd4[0]);
        close(fd5[1]);        
        }
        
        if (i == 2 && j == 2) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        n = read(fd5[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        //write(fd6[1],buffer,strlen(buffer));
        printf("%s  estoy en %d cuando i:%d y j:%d ",buffer, getpid(), i, j);
        close(fd5[0]);
        }
        
        
    }else{
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);

        /*
        n = read(fd6[0],buffer,sizeof(buffer));
        buffer[n] = '\0';
        */
        FILE *archivo = fopen("prueba.txt","r");
        
        if (!archivo) { //si archivo es NULL
            exit(-1);
        }
        fseek(archivo,0,2);
        int TAM = ftell(archivo), i;
        fseek(archivo,0,0);
        char info[TAM];
           
        for( i = 0; i < TAM; i++)
        {
            info[i] = getc(archivo);
        }
              
        write(fd1[1],&TAM,sizeof(int));
        //strcpy(buffer,"hola pipes");
        write(fd1[1], info,TAM);
       // printf("%s  estoy en %d cuando i:%d y j:%d ",buffer,getpid(),i,j);        
        fclose(archivo);
        close(fd1[1]);        
    }

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        
        for( i = 0; i < 3; i++)
        {
            wait(NULL);
        }
        
    }else{
        wait(NULL);
        if(i==2 && j==1) wait(NULL);
        sleep(1);  
    }
   //printf("%d e i: %d y j:%d \n", getpid(), i, j);
    return 0;
}
