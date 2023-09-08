#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void * oldhandler;
void sighandler(int sig)
{
	//no sé que poner xd;
}


int main( void ) 
{
	pid_t pidchild,pidchilds[2];
	oldhandler = signal(SIGUSR1,sighandler);
	int i = 0;
    int cantidad;
    printf("Digite numero de veces: ");
    scanf("%d",&cantidad);

	for(; i < 2 ; i++){
		 pidchild = fork();
		 if(!pidchild)
        {
            break;
        }
        else{
            pidchilds[i] = pidchild;
        }
	}

	 switch (pidchild)  
	{ 
        case -1:   
            printf("Error Fork.");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo   
                for(int q = 0; q < cantidad; q++)
                {   
        		    pause();
                    printf("[%d] hijo %d\n",getpid(),i+1);
                    if(kill(getppid(),SIGUSR1) == -1){
                        printf("Error al mandar la señal hijo-padre.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
        default: /// Bloque proceso padre 
        		usleep(300);
                for(int q = 0; q < cantidad; q++)
                {
                    printf("[%d] padre\n",getpid());
                    for(int i = 1; i>= 0;i--)
                    {
                    	if(kill(pidchilds[i],SIGUSR1) == -1)
                        {
                            printf("Error al mandar la señal.\n");
                            exit(EXIT_FAILURE);
                        }
                    	pause();
                    } 
                }
                printf("[%d] padre\n",getpid());
                signal(SIGUSR1,oldhandler); //lo dejo como estaba.

                //printf("endalizó el padre: %d\n", getpid());
 	}
    
}