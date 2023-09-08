#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define TAM 50

void *foo(void *param);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int palabras_total=0, lines_total=0; 

int main(int argc, char *argv[]){

	pthread_t idHilo[argc-1]; 

	int i;
	for(i=1; i<argc; i++){ 
		pthread_create(&idHilo[i-1], NULL, foo, (void *)argv[i]);
	}
	for(i=1; i<argc; i++){
		pthread_join(idHilo[i-1], NULL);
	}
	
	printf("Total lineas: %d\n", lines_total); 
	printf("Total palabras: %d\n", palabras_total); 

	return 0;
}

void *foo(void *param){
	char *filepath = (char *)param;
	char linea[TAM], c;
    int i, contador_linea=0, numeroPalabras=0, dentroPalabra=0;
    FILE *archivo;
    archivo = fopen(filepath, "r");
    
    i=0;
    
	while ( feof(archivo) == 0){
        c = fgetc(archivo);
		if(c == '\n'){
                    contador_linea++;
                }
        if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
        {
          if (dentroPalabra==0)
          {
            numeroPalabras += 1;
        dentroPalabra = 1;
          }
        }else {
            if (dentroPalabra == 1){
                dentroPalabra = 0; 
            }  
        }
    }
	printf("%s Lineas: %d palabras: %d [%u] \n", filepath, contador_linea, numeroPalabras, (unsigned int)pthread_self());
	pthread_mutex_lock(&mutex);
 	palabras_total = palabras_total + numeroPalabras;
 	lines_total= lines_total + contador_linea;
  	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}
