#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int totalLineas = 0;
int TotalPalabras = 0;

void * f_c(void* arg);

int main(int arg, char*  argv[]){
   if(arg==1){
   	printf("No existe archivo para analisar");
   }else{
   

	pthread_t *h;
	int i;
	h = (pthread_t*)calloc(arg,sizeof(pthread_t));

	for( i=1; i<arg; i++ ){
          pthread_create(&h[(i-1)],NULL,f_c,(void*)argv[i]);
	}

	for( i=0; i<(arg-1); i++ ){
            pthread_join(h[i],NULL);
        }

	printf("\nTotal de lineas : %d\n",totalLineas);
	printf("Total de palabras : %d\n",TotalPalabras);

    return 0;
}}

void * f_c(void* arg){

	char * nombre = (char*)arg;
	int contLineas=0;
	int contPalabras=0;

	FILE *archivo;
	archivo = fopen(nombre,"r");
	char caracter;
	int sw=0;
	if (archivo == NULL) exit(1);

	while (feof(archivo) == 0){

		caracter = fgetc(archivo);

		if(caracter){
				if(caracter==' '){
						if(sw==0){
							contPalabras++;
						}
						sw=1;
				}else if( caracter=='\n'){
						contLineas++;
						if(sw==0){
							contPalabras++;
						}
						sw=1;
				}else{
					sw=0;
				}

		}

	}

	pthread_mutex_lock(&mutex);
	   totalLineas = totalLineas + contLineas;
           TotalPalabras = TotalPalabras + contPalabras;
	pthread_mutex_unlock(&mutex);

	printf("%s Numero Lineas : %d ; Numero Palabras : %d ;  id[%lu]\n",nombre,contLineas,contPalabras,pthread_self());

	return NULL;
}
