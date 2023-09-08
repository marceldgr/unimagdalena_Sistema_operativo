#include <stdio.h> //IO
#include <stdlib.h> //Funciones del sistema
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
int main()
{
    int fd[2][2], i ,childs[2];
    pid_t padre = getpid();
    for( i = 0; i < 2; i++)
    {
        if (pipe(fd[i])) {
            printf("no se pudierron cargar las tuberias");
            exit(-1);
        }   
    }
    for(i = 0; i < 2; i++ ) if(!(childs[i]=fork())) break;

    if(padre != getpid()){
        if(i == 1){
            close(fd[0][0]);
            close(fd[1][0]);
            close(fd[1][1]);
            FILE *archivo = fopen("prueba.txt","r");
            if(!archivo){
                printf("error al abrir el archivo \n");
                exit(-1);
            }
            fseek(archivo,0,2);
            int TAM = ftell(archivo);
            fseek(archivo,0,0);
            char info[TAM];
            
            for( i = 0; i < TAM; i++)
            {
                info[i] = getc(archivo);
            }
            write(fd[0][1],&TAM,sizeof(int));
            write(fd[0][1],info,TAM);
            printf("%d: he terminado de enviar todo el archivo  \n", getpid());
            fclose(archivo);
            close(fd[0][1]);
        }
        
        if (i == 0) {
            close(fd[0][1]);
            close(fd[1][0]);
            int TAM,n_vocales = 0;
            read(fd[0][0],&TAM,sizeof(int));
            char info[TAM];
            read(fd[0][0],info,TAM);
            
            for( i = 0; i < TAM; i++)
            {
                switch(info[i]){
                    case 'a':
                    case 'e':
                    case 'i':
                    case 'o':
                    case 'u':
                    case 'A':
                    case 'E':
                    case 'I':
                    case 'O':
                    case 'U':
                        n_vocales++;
                        break;
                }
            }
            printf("%d: el numero de vocales es: %d \n",getpid(), n_vocales);
            write(fd[1][1],&TAM,sizeof(int));
            write(fd[1][1], info, TAM);
            
            close(fd[0][0]);
            close(fd[1][1]);
        }
        
    }else{
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][1]);
        int TAM,n_espacios = 0;
        read(fd[1][0],&TAM,sizeof(int));
        char info[TAM];
        read(fd[1][0],info,TAM);
        for(i = 0;i < TAM; i++){
            if(info[i] == '\n'){
                n_espacios++;
            }
        }
        printf("%d: numero de saltos de linea: %d \n",getpid(),n_espacios);
        close(fd[1][0]);
    }
    

    if(padre == getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        for( i = 0; i < 2; i++) wait(NULL);
    }else{
        sleep(2);  
    }
    return 0;
}
