#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define Buff_len 256

int main(void)
{
	pid_t root = getpid();
	int n;
	
	char buff[Buff_len];

	printf("\nIngrese la cantidad de hijos N: ");
	int N;
	scanf("%d", &N);

	int N_pipes = N + 1;
	int pipes[N_pipes][2];

	for (int i = 0; i < N_pipes; i++)
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("Error al crear pipes...\n");
		}
	}
	

	pid_t pid_Hijo;
	for (int i = 0; i < N; ++i)
	{
		if(fork()==0)
		{
			pid_Hijo = i;
			break;
		}
	}

	if (root == getpid())
	{
		printf("\nIngrese el mensaje: ");
		char msg[Buff_len];
		scanf("%s", msg);
		
		close(pipes[0][0]);
		write(pipes[0][1], msg, strlen(msg));
		printf("Padre:[%d] escribiendo mensaje en la tuberia: --> %s\n", getpid(), msg);
		close(pipes[0][1]);

		close(pipes[(N_pipes-1)][1]);
		n = read(pipes[(N_pipes-1)][0], buff, Buff_len);
		buff[n] = '\0';
		printf("\nPadre:[%d] leyendo mensaje de la tuberia: <-- %s\n", getpid(), buff);
		close(pipes[(N_pipes-1)][0]);

		for (int i = 0; i < N; i++)
		{
			wait(NULL);
		}

	}else{

		close(pipes[pid_Hijo][1]);
		n = read(pipes[pid_Hijo][0], buff, Buff_len);
		buff[n] = '\0';
		printf("\nHijo:[%d] leyendo mensaje de la tuberia: <-- %s\n", getpid(), buff);
		close(pipes[pid_Hijo][0]);

		char msg[Buff_len];
		strcpy(msg, buff);
		close(pipes[(pid_Hijo+1)][0]);
		write(pipes[(pid_Hijo+1)][1], msg, strlen(msg));
		printf("Hijo:[%d] escribiendo mensaje en la tuberia: --> %s\n", getpid(), msg);
		close(pipes[(pid_Hijo+1)][1]);

	}
	return EXIT_SUCCESS;
}