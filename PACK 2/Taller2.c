/*
	Taller número 2 - Sistemas Operativos.

	Autores:
	Ingris Triana,
	Kevin Moreno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

void * oldhandler;

void sighandler(int sig){}

int main() {
    pid_t pidhijo[3], root;
	int i, j;

	oldhandler = signal(SIGUSR1, sighandler);
	if(oldhandler == SIG_ERR) { perror("Signal:"); exit(EXIT_FAILURE);}

	root = getpid();
	for(i = 0; i < 3; i ++){
		pidhijo[i] = fork();
		if(!pidhijo[i]) break;
	}

	if(root == getpid()){
		printf("a [%d] \n", getpid());
		usleep(1000);
		kill(pidhijo[2], SIGUSR1);
		pause();
		printf("a [%d] \n", getpid());
		for(j = 0; j < 3; j++) wait(NULL);
	}
	else{
		pause();
		
		if(i == 2){
			printf("d [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
			kill(pidhijo[1], SIGUSR1);
		}
		else if(i == 1){
			
			pid_t pidHijoE = fork();
			
			if(pidHijoE == 0) {
				pause();
				pid_t pidHijoF = fork();

				if(pidHijoF == 0) {
					pause();
					pid_t pidHijoG = fork();

					if(pidHijoG == 0) {
						pause();
						printf("g [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
						kill(getppid(), SIGUSR1);
					} else {
						printf("f [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
						usleep(1000);
						kill(pidHijoG, SIGUSR1);
						pause();
						printf("f [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
						kill(getppid(), SIGUSR1);
						wait(NULL);
					}
				} else {
					printf("e [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
					usleep(1000);
					kill(pidHijoF, SIGUSR1);
					pause();
					printf("e [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
					kill(getppid(), SIGUSR1);
					wait(NULL);
				}
			} else {
				printf("c [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
				usleep(100);
				kill(pidHijoE, SIGUSR1);
				pause();
				printf("c [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
				kill(pidhijo[0], SIGUSR1);
				wait(NULL);
			}
		} 
		else if(i == 0){
			printf("b [%d] <- Hijo, Padre -> [%d]\n", getpid(), getppid());
			kill(getppid(), SIGUSR1);
		}
	}
	return EXIT_SUCCESS;
}