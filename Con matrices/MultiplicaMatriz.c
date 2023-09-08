#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


	 /////		RICARDO VALENCIA QUINTERO 2010 114 109
	/////		DORA ZULEMA OSPITIA	2009 114 069



void* func_hilo(void*);
void* func_hilo2(void*);
//// se declaran dobles punteros para posteriormente convertirlos en matrices
int **A;
int **B;
int **C;


//mutex de turno para los hilos de las operaciones y el final para imprimir la matriz
pthread_mutex_t mutexTurno = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexFinal = PTHREAD_MUTEX_INITIALIZER;
//variables de condicion para el turno y para que el tercer hilo no imprima hasta que termine toda la operacion
pthread_cond_t condTurno = PTHREAD_COND_INITIALIZER;
pthread_cond_t finaliza = PTHREAD_COND_INITIALIZER;

int myturn = 0, af ,ac ,bf, bc;

int main(int argc, char const *argv[])
{
	int i, j, *vecIndices;
	pthread_t *vecHiloId = NULL;

	///// se piden las filas y columnas y se validan
	printf("Multiplicación de matrices con hilos\n");
	printf("\nDigite el número de FILAS de la PRIMERA matriz\n");
	scanf("%d", &af);
	printf("\nDigite el número de COLUMNAS de la PRIMERA matriz\n");
	scanf("%d", &ac);

	printf("\nDigite el número de FILAS de la SEGUNDA matriz\n");
	scanf("%d", &bf);
	printf("\nDigite el número de COLUMNAS de la SEGUNDA matriz\n");
	scanf("%d", &bc);

	/// se valida si se puede realizar la multiplicación de la matriz

	if (ac != bf){
		printf("tamaños incorrectos no se puede multiplicar...\n");
		exit(0);
	}
	// se crean las matrices de manera dinámica
	A = (int **) calloc(af, sizeof(int*));
	for (i = 0; i < af; i++)
		A[i] = (int *) calloc(ac, sizeof(int));
	
	B = (int **) calloc(bf, sizeof(int*));
	for (i = 0; i < bf; i++)
		B[i] = (int *) calloc(bc, sizeof(int));

	C = (int **) calloc(af, sizeof(int*));
	for (i = 0; i < af; i++)
		C[i] = (int *) calloc(bc, sizeof(int));
	
	//// procede con el llenado de las matrices
	printf("Digite valores para la primera matriz\n");
	for (i = 0; i < af; i++){
		for (j = 0; j < ac; j++){
			printf("Ingrese el valor de la pos [%d][%d]\n",i+1, j+1);
			scanf("%d", &A[i][j]);
			C[i][j] = 0;
		}
	}

	printf("Digite valores para la segunda matriz\n");
	for (i = 0; i < af; i++){
		for (j = 0; j < ac; j++){
			printf("Ingrese el valor de la pos [%d][%d]\n",i+1, j+1);
			scanf("%d", &B[i][j]);
		}
	}

	///vectores dinámicos (innecesarios pero bueno)
	vecIndices = (int*) calloc(3, sizeof(int));
	vecHiloId = (pthread_t*) calloc(3, sizeof(pthread_t));

	/// creacion de los hilos, los dos primeros operan, el tercero imprime
	for (i = 0; i < 3; i++){
		vecIndices[i] = i;
		if (i < 2)	{pthread_create(&vecHiloId[i], NULL, func_hilo, (void*)&vecIndices[i]);}
		else pthread_create(&vecHiloId[i], NULL, func_hilo2, NULL);		
	}
	//// espera los hilos
	for (i = 0; i < 3; i++){
		pthread_join(vecHiloId[i], NULL);
	}

	return 0;
}

		////la func_hilo hace que los dos primeros hilos multipliquen las matrices

void* func_hilo(void* param){//el hilo 0 calcula impares; el hilo 1 calcula pares
	int p = *(int*)param;
	int i = 0, j = 0, k = 0;

	for (i = 0; i < af; i++){
		for (j = 0; j < bc; j++){
			pthread_mutex_lock(&mutexTurno);//el mutex después de los dos primeros for para que el otro hilo pueda cambiar los valores
				while((i + j % 2) == 0 && p == 0) // si la suma es par y es el hilo 0 no es su turno
					pthread_cond_wait(&condTurno, &mutexTurno);
				for (k = 0; k < ac; k++){
					C[i][j] += A[i][k] * B[k][j];// multi
				}
				pthread_cond_signal(&condTurno);
			pthread_mutex_unlock(&mutexTurno);
		}	
	}

	if (p == 1){// para que solo uno entre y el otro no se bloquee
		pthread_mutex_lock(&mutexFinal);
			myturn = 1;
			pthread_cond_signal(&finaliza);
		pthread_mutex_unlock(&mutexFinal);
	}
	

	pthread_exit(0);
}

void* func_hilo2(void* param){
	int i, m;
	pthread_mutex_lock(&mutexFinal);
		while(myturn == 0) // espera para que los hilos finalicen y el pueda imprimir
			pthread_cond_wait(&finaliza, &mutexFinal);

		for (i = 0; i < af; i++){
			printf("\t");
			for (m = 0; m < bc; m++){
				printf("[%d]\t",C[i][m]);
			}
			printf("\n");
		}
	pthread_mutex_unlock(&mutexFinal);

	pthread_exit(0);
}