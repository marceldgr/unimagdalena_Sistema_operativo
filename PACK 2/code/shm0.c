#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>


int main(){

	int shmid, max_iter = 10000000;
	int * pointer;

	shmid   = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|S_IRUSR|S_IWUSR);
	pointer = (int *)shmat(shmid, 0, 0);
	*pointer = 0;

	if(!fork()){		
		for (int i = 0; i < max_iter; ++i)
		{
			*pointer = *pointer + 1;
		}
		shmdt(pointer);
	}
	else{
		for (int i = 0; i < max_iter; ++i)
		{
			*pointer = *pointer + 1;
		}
		wait(NULL);
		printf("the value of pointer = [%d] %d\n", *pointer, (int)sizeof(int));

		shmdt(pointer);
		shmctl(shmid, IPC_RMID, 0);
	}

	return EXIT_SUCCESS;
}