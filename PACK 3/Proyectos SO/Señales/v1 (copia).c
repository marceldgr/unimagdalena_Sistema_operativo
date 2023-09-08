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
        		pause();
                printf("[%d] hijo %d\n",getpid(),i+1);
                break;
        default: /// Bloque proceso padre 
        		usleep(300);
        		printf("[%d] padre\n",getpid());
                for(int i = 1; i>= 0;i--)
                {
                	kill(pidchilds[i],SIGUSR1);
                	wait(NULL);
                }
                printf("[%d] padre\n",getpid());

                //lo dejo como estaba
                if(signal(SIGUSR1,oldhandler) == SIG_ERR){
                	perror("Signal:");
        			exit(EXIT_FAILURE);
                }
                
 	}

}