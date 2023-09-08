#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    int n,i,j = -1;
    printf("ingrese el tamaño : \n");
    scanf("%d", &n);
    int fd[n][2];
    pid_t childs[n], padre = getpid();

    
    for( i = 0; i < n; i++)
    {
        if(pipe(fd[i])){
            printf("no se pudieron crear las tuberias");
            exit(-1);
        }
    }
    
    for( i = 0; i < n; i++)
    {
        if(!(childs[i] = fork())) break;
    }
    
    if(getpid() != padre){

        for( j = 0; j < n; j++)
        {
            if(i != j) close(fd[j][0]);
            if((i+1) != j) close(fd[j][1]);
        }
        int TAM;
        read(fd[i][0], &TAM,sizeof(int));
        char info[TAM];
        read(fd[i][0],info,TAM);
        printf("%d %s \n",getpid(), info);
        write(fd[i+1][1], &TAM, sizeof(int));
        write(fd[i+1][1], info,TAM);
        close(fd[i][0]);
        close(fd[i+1][1]);

    }else{       
        for( j = 0; j < n; j++)
        {            
            if (j != 0) {
            close(fd[j][0]);
            close(fd[j][1]);
            }else{
                close(fd[j][0]);
            }
        }

        FILE *archivo = fopen("prueba.txt","r");
        if(!archivo){
            printf("error al abrir el archivo \n");
            return -1;
        }
        fseek(archivo,0,2);
        int TAM = ftell(archivo);
        fseek(archivo,0,0);
        TAM -= 1;
        char info[TAM];
        
        for( i = 0; i < TAM; i++)
        {
            info[i] = getc(archivo);
        }
        
        write(fd[0][1],&TAM,sizeof(int));
        write(fd[0][1],info,TAM);
        fclose(archivo);
        close(fd[0][1]);     
    }
    
    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        for( i = 0; i < n; i++) wait(NULL);
    }else{
        sleep(2);  
    }
    return 0;
}
