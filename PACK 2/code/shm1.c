
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main(){
	int *ptr=NULL;
	int shm_id, c=0;
	int shm_size = sizeof(int);

	shm_id=shmget(IPC_PRIVATE, shm_size, IPC_CREAT | S_IRUSR | S_IWUSR);
	ptr = (int *) shmat(shm_id, 0, 0);		
	*ptr=0;
		
	if(fork() ==0 ){				
		printf("[%d]child process started\n", getpid() );		
		ptr = (int *) shmat(shm_id, 0, 0);		
		do{}while(*ptr == 0); 
		printf("[%d]shm_value->%d\n", getpid(),*ptr);				
		
		shmdt(ptr);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		sleep(2);
		*ptr = 500; //write into shm
		printf("[%d]shm_value->%d\n", getpid(),*ptr);				
		wait(NULL);

		
		shmdt(ptr);
		shmctl(shm_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}

