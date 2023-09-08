#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <math.h>
#include <signal.h>

void sig_handler(int s){};
unsigned int sizeof_dm(int, int, size_t);
void create_index(void **, int, int, size_t);

int main(int argc, char const *argv[])
{
	pid_t child; 
	int shmId;

	int Rows=3, Cols=2;
	double **matrix;

	size_t sizeMatrix = sizeof_dm(Rows, Cols, sizeof(double));
	shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
	

	signal(SIGUSR1, sig_handler);

	child = fork();
	if(!child){
		printf("[%d]child process started\n", getpid());
		matrix = shmat(shmId, 0, 0);
		pause();
		

		for (int i=0; i < Rows; ++i)
		{
			for (int j=0; j < Cols; ++j)
			{
				printf(" %f ", matrix[i][j]);
			}
			printf("\n");
		}

		shmdt(matrix);
	}
	else{
		printf("[%d]parent process started\n", getpid() );
		matrix = shmat(shmId, 0, 0);
		create_index((void*) matrix, Rows, Cols, sizeof(double));

		for (int i=0; i < Rows; ++i)
		{
			for (int j=0; j < Cols; ++j)
			{
				matrix[i][j] = rand()%100;
			}
		}

		usleep(8000);
		kill(child, SIGUSR1);
		wait(NULL);
		
		shmdt(matrix);
		shmctl(shmId, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
	size_t size = rows * sizeof(void *); //indexSize
	size+= (cols * rows * sizeElement); //Data size
	return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
	int i;
	size_t sizeRow = cols * sizeElement;
	m[0] = m + (rows * sizeof(void *));
	for(i=1; i<rows; i++){
		m[i] = (m[i-1] + sizeRow);
	}
}
