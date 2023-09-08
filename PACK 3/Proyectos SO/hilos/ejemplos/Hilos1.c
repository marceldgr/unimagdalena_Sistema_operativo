#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* funcion_hilo(void*);

int main(){
	pthread_t tid;
	pthread_create(&tid, NULL, funcion_hilo, NULL);
	printf("Hilo principal[%lu]\n", pthread_self());
	pthread_join(tid, NULL);
	return EXIT_SUCCESS;
}

void* funcion_hilo(void* arg){
	printf("Hilo [%lu]\n", pthread_self());
	pthread_exit(0);
}
