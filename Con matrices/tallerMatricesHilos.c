#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <pthread.h>
void * funcionhilo(void *);
void retardo(int);
pthread_mutex_t  myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condicion = PTHREAD_COND_INITIALIZER;


int matriz1[10][10], matriz2[10][10], matrizR[10][10];
int tam=0, turno=0;
int main(){
    int  i=0, j=0, k=0;
    int valor = 0;
    pthread_t *pidhilo=NULL;
   
    printf("Ingrese el tamaño de las matrices.... ");
    scanf("%d",&tam);
    matriz1[1][1] = matriz1[tam][tam] ;
    matriz2[1][1] = matriz2[tam][tam] ;
    matrizR [1][1] = matrizR[tam][tam];
    

    pidhilo = (pthread_t *) calloc(tam, sizeof(pthread_t));
   
    for(i=0;i<tam; i++){       
        for(j=0;j<tam; j++){
            printf("ingresa un valor para la primera matriz: ");
            scanf("%d",&valor);
            matriz1[i][j] = valor;
        }   
       
    }
    printf("\n");
    for (i=0;i<tam;i++){
        for(j=0; j<tam; j++){
            printf("ingresa un valor para la segunda matriz: ");
            scanf("%d",&valor);
            matriz2[i][j] = valor;
        }   
    }

   
    for(i=0;i<=tam;i++) pthread_create(&pidhilo[i], NULL, funcionhilo,(void*)i);
           
    
   
    for(i=0;i<=tam;i++) pthread_join(pidhilo[i],NULL);
   

    free(pidhilo);
    return 0;
}

void * funcionhilo(void *param){
    int miTurno= (int)param;
    int  i=0, k=0, j=0;
    pthread_mutex_lock(&myMutex);
    while(turno!=miTurno) pthread_cond_wait(&condicion,&myMutex);
    if (miTurno<tam)
    {
        for (i = 0; i < tam; i++)
        {
       
       matrizR[i][miTurno] = 0;
       for (k = 0; k < tam; k++)
       {
           matrizR[i][miTurno] += matriz1[i][k]*matriz2[k][miTurno];

       }
    }
    
       
    }else{
        //recorrer matriz resultante e imprimirla en el último hilo
        for (i = 0; i < miTurno; i++)
        {   printf("[");
            for (j = 0; j < miTurno; j++)
            {
                printf("%d ", matrizR[i][j]);
            }
            printf("]\n");
        }

    }
    
      
    turno++;
           
    pthread_cond_broadcast(&condicion);
    pthread_mutex_unlock(&myMutex);
    retardo(1000);

    pthread_exit(0);
   
}
void retardo(int n){
    usleep(n);
}
