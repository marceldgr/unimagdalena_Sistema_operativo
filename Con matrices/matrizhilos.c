#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* func_hilo(void*);

int A[3][3] = {{1, 2, 3},{1, 2, 3}, {1, 2, 3}};
int B[3][3] = {{1, 2, 3},{1, 2, 3}, {1, 2, 3}};
int C[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int myturn = 0;

int main(int argc, char const *argv[])
{

	int i = 0, m, *vecIndices, nhilos = 3;
	pthread_t *vecHiloId = NULL;

	vecIndices = (int*) calloc(nhilos, sizeof(int));
	vecHiloId = (pthread_t*) calloc(nhilos, sizeof(pthread_t));

	for (i = 0; i < nhilos; i++){
		vecIndices[i] = i;
		pthread_create(&vecHiloId[i], NULL, func_hilo, (void*)&vecIndices[i]);
	}

	for (i = 0; i < nhilos; i++){
			printf("\t");
		for (m = 0; m < nhilos; m++){
			printf("[%d]\t",C[i][m]);
		}
		printf("\n");
	}

	for (i = 0; i < nhilos; i++){
		pthread_join(vecHiloId[i], NULL);
	}

	

	return 0;
}

void* func_hilo(void* param){//hay que enviarle un vec o estructura (i, nhilos)
	int p = *(int*)param;
	int j = 0, k = 0;

	pthread_mutex_lock(&mutex);
		for (j = 0; j < 3; j++){
			if (j > p){// el problema es que no son diferentes
				break;
			}
			while(j != myturn)//puede ser el mismo p???   sin son iguales no cumple
				//if (j > myturn)	break;
				pthread_cond_wait(&cond, &mutex);

			for (k = 0; k < 3; k++){
				C[p][j] += A[p][k]*B[k][j];
			}

			myturn ++;	
		}

		pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}