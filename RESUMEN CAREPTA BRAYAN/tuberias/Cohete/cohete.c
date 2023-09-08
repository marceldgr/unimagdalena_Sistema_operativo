#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

int main()
{
    char buffer[1024];
    
    pid_t padre = getpid();
    int fd[4][2], i, n;
    int bandera = 0;
    for( i = 0; i < 4; i++)
    {
        if(pipe(fd[i])){
            printf("no se pudieron crear las tuberias");
            exit(-1);
        }        
    }        
    for( i = 0; i < 2; i++) if(!fork())break;

    
    if (getpid() != padre) {
        if(i == 0){
            for(int j = 0; j < 4; j++)
            {
                if(j != 1)close(fd[j][0]);
                if(j != 0)close(fd[j][1]);
            }
            int buster[4];
            
            srand(time(NULL));
            for(int a = 0; a < 4; a++){
                buster[a] = rand()%7;
            }
            for(int j = 0; j < 4; j++)
            {
                printf("%d\n",buster[j]);
                if(buster[j] > 5){
                    bandera = 1;
                    write(fd[0][1],&bandera,sizeof(int));
                    strcpy(buffer,"Exedio la temperatura! va a explotar!!!");
                    write(fd[0][1],buffer,strlen(buffer));
                    break;
                }
            }
            write(fd[0][1],&bandera,sizeof(int));
            n = read(fd[1][0],buffer, sizeof(buffer));
            buffer[n] = '\0';
            printf("hijo%d dice: %s\n ",i+1, buffer);
            close(fd[1][0]);
            close(fd[0][1]);    
        }
        
        if (i == 1) {
            for(int j = 0; j < 4; j++)
            {
                if(j != 3)close(fd[j][0]);
                close(fd[j][1]);
            }
            n = read(fd[3][0],buffer, sizeof(buffer));
            buffer[n] = '\0';
            printf("hijo%d dice:%s\n",i+1, buffer);
            close(fd[3][0]);
        }
        
    }else{//padre
        for(int j = 0; j < 4; j++)
        {
           if(j != 0 && j != 2)close(fd[j][0]);
           if(j != 1 && j != 3)close(fd[j][1]);
        }
        int inicio = time(NULL);
        int final = inicio;
       
        
        do{
            final= time(NULL); 
            read(fd[0][0],&bandera,sizeof(int));
            
            if(bandera == 1){
               
               n = read(fd[0][0],buffer, sizeof(buffer));
               buffer[n] = '\0';
               printf("%s\n", buffer);
               
               break;
            }
            printf("en espera 5 segundos...\n");
            sleep(5);
           break;
        }while((final-inicio)<5);
        //printf("bandera es :%d\n",bandera);
        if (bandera == 0){
            printf("morira proceso 1\n");
            strcpy(buffer,"muere proceso");
            write(fd[1][1],buffer,strlen(buffer));
           
            strcpy(buffer,"lanzar capsula");
            write(fd[3][1],buffer,strlen(buffer));
        }else{
            //orden de abortar
            printf("padre: notificacion de abortar\n");
            strcpy(buffer,"abortar, lanzare la capsula con los astronautas!");
            write(fd[3][1],buffer,strlen(buffer));
            printf("padre: morira proceso 1\n");
            strcpy(buffer,"muere proceso");
            write(fd[1][1],buffer,strlen(buffer));
        }
        close(fd[0][0]);
        close(fd[2][0]);
        close(fd[1][1]);
        close(fd[3][1]);
    }

    return 0;
}
