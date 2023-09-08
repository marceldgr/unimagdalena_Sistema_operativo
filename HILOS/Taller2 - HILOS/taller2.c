#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct Lista {
    int info;
    struct Lista *sig;
};

struct Result{
    int nTimes;
    struct Lista *indexes;
    int thread;
};

int *vector,size,n,find;
struct Result *result;

void *thread_routine(void * arg){
    int * index = (int*) arg;
    int segmento = size/n;
    int start = segmento*(*index);
    int end = segmento*(*index+1);

    if (size%n != 0){
        if ((n-1) == *index && end < size){
            int faltante = size - end;
            end += faltante;
            segmento += faltante;
        }
    }

    result[*index].nTimes = 0;
    result[*index].thread = *index;

    struct Lista *ptr = NULL, *last ,*nuevo;

    for (int i=start; i<end; i++){
        if(vector[i]==find){

            nuevo = (struct Lista *) malloc (sizeof(struct Lista));
            nuevo->sig = NULL;

            result[*index].nTimes++;
            nuevo->info = i;
            //printf("nuevo %d\n",nuevo->info);
            if (ptr == NULL){
                ptr = nuevo;
                last = nuevo;
            }else{
                last->sig = nuevo;
                last = nuevo;
            }
        }
    }

    if (result[*index].nTimes > 0){
        result[*index].indexes = ptr;
    }else{
        result[*index].indexes = NULL;
    }
}

int main(){
    system("clear");
    pthread_t *threads;
    FILE *file = fopen("vector.txt","r");

    if(!file){
        printf("Error al abrir el archivo");
        exit(-1);
    }

    fscanf(file,"%d",&size);

    vector = (int*) malloc(sizeof(int)*size);

    printf("Este es el vector cargado del archivo: \n");
    for(int i=0;i<size;i++){
        fscanf(file,"%d",&vector[i]);
        printf("%d\t",vector[i]);
    }
    printf("\n\n");

    printf("Ingrese el numero de hilos:");
    scanf("%d",&n);

    if (n > size){
        printf("El numero de hilos es mayor al del vector. Abortando..\n");
        exit(-1);
    }

    result = (struct Result*) malloc(sizeof(struct Result)*(n));

    printf("Ingrese el numero a buscar:");
    scanf("%d",&find);
    printf("\n");

    threads = (pthread_t*) malloc(sizeof(pthread_t)*n);
    int indexes[n];
    for(int j=0;j<n;j++){
        indexes[j] = j;
        pthread_create(&threads[j],NULL,thread_routine,(void*) &indexes[j]);
    }

    for(int i=0;i<n;i++)
        pthread_join(threads[i],NULL);


    for(int i=0;i<n;i++){
        printf("El hilo %d encontro %d veces el numero\n",result[i].thread,result[i].nTimes);
        if(result[i].indexes != NULL){
            printf("\tEn las posiciones: ");
            struct Lista *auxiliar; /* lo usamos para recorrer la lista */
            auxiliar = result[i].indexes;
            while (auxiliar!=NULL) {
                  printf("%d ",auxiliar->info);
                  auxiliar = auxiliar->sig;
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}
