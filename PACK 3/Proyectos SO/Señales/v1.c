#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#define CHILDS 4

void * oldhandler;
void sighandler(int sig)
{
	//no sé que poner xd;
}

int main( void ) 
{
	pid_t pidchild,pidchilds[CHILDS];
	oldhandler = signal(SIGUSR1,sighandler);

	if(oldhandler == SIG_ERR){
        perror("Signal:");
        exit(EXIT_FAILURE);
    }

	int i = 0;

	for(; i < CHILDS ; i++){
		 pidchild = fork();

		 if(!pidchild)
        {
            break;
        }
        else
        {
            pidchilds[i] = pidchild;
        }
	}

	 switch (pidchild)  
	{ 
        case -1:   
            printf("Error Fork.");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo   
        		pause();
        		printf("[%d] hijo %d\n",getpid(),i+1);
        		if(i > 0){
        		   if(kill(pidchilds[i-1],SIGUSR1)){
        		   		printf("Error al mandar la señal.\n");
                        exit(EXIT_FAILURE);
        		   }
        		}
        		else{
        			if(kill(getppid(),SIGUSR1)){
        		   		printf("Error al mandar la señal.\n");
                        exit(EXIT_FAILURE);
        		   }
        		}

                break;
        default: /// Bloque proceso padre 
        		usleep(400);
        		printf("[%d] padre\n",getpid());
                if(kill(pidchilds[CHILDS-1],SIGUSR1))
                {
                	printf("Error al mandar la señal.\n");
                    exit(EXIT_FAILURE);
                }
                pause();
                printf("[%d] padre\n",getpid());
                for(i = 0; i < CHILDS; i++){
                	wait(NULL);
                }

                //lo dejo como estaba
                if(signal(SIGUSR1,oldhandler) == SIG_ERR){
                	perror("Signal:");
        			exit(EXIT_FAILURE);
                }
                
 	}

}