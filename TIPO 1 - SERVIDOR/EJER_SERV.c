#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


void *oldhandler;
void sighandler(int sig){
printf("[%d]sig %d captutadan",getpid(),sig);
}


int main()
{
int i=0;
int j,k,n=0;
pid_t pid[5];
pid_t h0[i];
pid_t h211;



oldhandler=signal(SIGUSR1,sighandler);

if(oldhandler==SIG_ERR){perror("signal:\n");exit(EXIT_FAILURE);}

for(i=0;i<5;i++){
	pid[i]=fork();
	if(!pid[i]){
	printf("pid [%d] con el pid %d\n",i,getpid());
	
	if(i==0){
		h0[i]=fork();
		if(!h0[i]){
		printf("pid 01 con el pid %d\n",getpid());
		}
	}
	if(i==2){
		h0[i]=fork();
		if(!h0[1]){

		printf("pid 12 con el pid %d\n",getpid());
		h211=fork();
		if(!h211){
		printf("pid 211 con el pid %d\n",getpid());
		}
		}
		
	}
	
	if(i==4){
		h0[i]=fork();
		if(!h0[4]){
		printf("pid 01 con el pid %d\n",getpid());
		}

	}
	break;
	}
}


if(i==3){
	usleep(1000);
	printf("padre con el pid [%d] inicio\n",getpid());
	kill(pid[4],SIGUSR1);
	pause();
	printf("padre con el pid [%d] termino\n",getpid());
	for(i=0;i<5;i++){wait(NULL);}
}
else{
	
	pause();
	if(i!=0){
	
	
		if(pid[i]==2){
		if(!h0[i]==2){
		
	       if(!h211){
		printf("hijo [%d] termino\n",i);	
		usleep(1000);
	        kill(h211,SIGUSR1);
	        pause();
	        printf("hijo [%d] termino la segunda vez\n",i);	
	        
		}else{
		printf("hijo 21 termino\n");	
		usleep(1000);
	        kill(getppid(),SIGUSR1);
	        pause();
		
		}
		}}else{
		if(!h0[i]){		
		printf("hijo [%d] termino\n",i);	
		usleep(1000);
	        kill(h0[i],SIGUSR1);
	        pause();
	        printf("hijo [%d] termino por segunda vez\n",i);
	        usleep(1000);
	        kill(pid[i-1],SIGUSR1);
		}else{
		printf("hijo [%d] termino\n",i);
		usleep(1000);
	        kill(getppid(),SIGUSR1);
		}}
	
	
	
	
	}else{
	printf("hijo [%d] termino\n",i);
	usleep(1000);
	kill(getppid(),SIGUSR1);
	
	}
		
		
		
	
	}
	
return EXIT_SUCCESS;
}







