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

    if(oldhandler == SIG_ERR){
        perror("Signal:");
        exit(EXIT_FAILURE);
    }

	int i = 0;
    int cantidad;

    printf("Digite numero de veces: ");
    scanf("%d",&cantidad);

    if(cantidad <= 0){
        exit(0);
    }
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
                    if(i == 1)
                    {
                       if(kill(pidchilds[0],SIGUSR1) == -1)
                       {
                            printf("Error al mandar la señal.\n");
                            exit(EXIT_FAILURE);
                       }
                    }
                    
                    else
                    {
                        if(kill(getppid(),SIGUSR1) == -1)
                        {
                            printf("Error al mandar la señal.\n");
                            exit(EXIT_FAILURE);
                       }    
                    }
                }
                break;
        default: /// Bloque proceso padre 
        		usleep(300);
                for(int q = 0; q < cantidad; q++)
                {
                    printf("[%d] padre\n",getpid());

                    if(kill(pidchilds[1],SIGUSR1) == -1)
                    {
                            printf("Error al mandar la señal.\n");
                            exit(EXIT_FAILURE);
                    }
                    pause();
                    
                }
                printf("[%d] padre\n",getpid());
                for(i = 0; i<2; i++){
                    wait(NULL);
                }

                if(signal(SIGUSR1,oldhandler) == SIG_ERR){
                    perror("Signal:");
                    exit(EXIT_FAILURE);
                }
                //printf("endalizó el padre: %d\n", getpid());
 	}
    
}