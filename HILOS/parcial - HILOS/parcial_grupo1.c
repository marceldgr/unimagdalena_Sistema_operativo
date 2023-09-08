#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int nhilos, nintervalos, nfilas, ncolumnas;
double **placa, **matrizAux, ***estados;
int v=0, nHilosTerminaronLaIteracion=0;

void imprimir_matriz(double **m, int nfilas, int ncolumnas){
    for (int f=0; f<nfilas; f++){
        for (int c=0; c<ncolumnas; c++){
            printf("%.2f\t", m[f][c]);
        }
        printf("\n");
    }
    printf("\n");
}

double calcularTemperatura(double **m, int f, int c){
    double sum = m[f-1][c] + m[f+1][c] + m[f][c-1] + m[f][c+1];
    return sum/4;
}

void guardarEstado(double **m, int nfilas, int ncolumnas, int nIteracion){
    double **estado = (double**)malloc(nfilas*sizeof(double*));
    for (int k=0; k<nfilas; k++)
        estado[k] = (double*)malloc(ncolumnas*sizeof(double));

    for (int f=0; f<nfilas; f++){
        for (int c=0; c<ncolumnas; c++){
            estado[f][c] = m[f][c];
        }
    }
    estados[nIteracion] = estado;
}

void *thread_routine(void *arg){
    int *i = (int*)arg;
    int miNumeroDeIteracion=0;
    int segmento = (nfilas-2)/nhilos;
    int inicio = segmento * (*i);
    int fin = segmento * (*i+1);

    while(v < nintervalos){

        if (miNumeroDeIteracion >= nintervalos) break;
        while (v != miNumeroDeIteracion) sleep(1);

        for (int f=inicio+1; f<fin+1; f++){
            for (int c=1; c<ncolumnas-1; c++){
                matrizAux[f][c] = calcularTemperatura(placa, f, c);
            }
        }
        miNumeroDeIteracion++;
        nHilosTerminaronLaIteracion++;

        if (nHilosTerminaronLaIteracion == nhilos){
            for (int f=1; f<nfilas-1; f++){
                for (int c=1; c<ncolumnas-1; c++){
                    placa[f][c] = matrizAux[f][c];
                }
            }

            guardarEstado(placa, nfilas, ncolumnas, v);
            v++;
            nHilosTerminaronLaIteracion = 0;
        }
    }
}

int main(){
    system("clear");
    pthread_t *threads;

    FILE *file = fopen("parcial_g1.txt", "r");
    if (!file){
        printf("Error al abrir el archivo. Abortando....\n");
        exit(-1);
    }
    fscanf(file, "%d %d", &nfilas, &ncolumnas);

    placa = (double**)malloc(nfilas*sizeof(double*));
    for (int k=0; k<nfilas; k++)
        placa[k] = (double*)malloc(ncolumnas*sizeof(double));

    matrizAux = (double**)malloc(nfilas*sizeof(double*));
    for (int k=0; k<nfilas; k++)
        matrizAux[k] = (double*)malloc(ncolumnas*sizeof(double));

    float leido;
    for (int f=0; f<nfilas; f++){
        for (int c=0; c<ncolumnas; c++){
            fscanf(file, "%f", &leido);
            placa[f][c] = (double)leido;
        }
    }

    printf("Esta es la placa inicialmente.\n");
    imprimir_matriz(placa, nfilas, ncolumnas);

    printf("Ingrese el numero de hilos: ");
    scanf("%d", &nhilos);

    if ((nfilas-2)%nhilos != 0){
        printf("El numero de hilos no es divisible entre el numero "
                "de filas que es %d\n", nfilas-2);
        exit(-1);
    }

    printf("Ingrese el numero de intervalos: ");
    scanf("%d", &nintervalos);

    threads = (pthread_t*)malloc(nhilos*sizeof(pthread_t));
    estados = (double***)malloc(nintervalos*sizeof(double**));
    int values[nhilos];

    for (int i=0; i<nhilos; i++){
        values[i] = i;
        pthread_create(&threads[i], NULL, thread_routine, (void*)&values[i]);
    }

    for (int i=0; i<nhilos; i++) pthread_join(threads[i], NULL);

    printf("\nHilo principal [%lu]: Estos son los estados.\n\n", pthread_self());

    for (int x=0; x<nintervalos; x++){
        printf("========================== Estado #%d de la placa ==========================\n", x+1);
        imprimir_matriz(estados[x], nfilas, ncolumnas);
    }

    return 0;
}
