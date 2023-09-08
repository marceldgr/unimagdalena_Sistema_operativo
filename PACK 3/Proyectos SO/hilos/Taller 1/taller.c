/*
Taller de hilos
Elaborado por:
    Daniel Miguel Vega Camargo:    2017214084 | Grupo 1
    Luis Armando Zuñiga Paredes:   2015114089 | Grupo 2

*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define FILE_NAME "archivo.txt"

struct DatoHilo
{
    int id;
    int F_ini;//fila de inicio
    int F_fin; //fila de finalización.
};

void * TrabajoHilo(void *);
void TrabajoHiloPrincipal(void);
void asignarRangos(int,int*,int*);
float calcularValorPunto(int x, int y);
void recorrerRejilla(int inicio,int fin);

void leerArchivo(char*);
float **crearMatrizDinamica(int,int);

void ErrorReport(char* msg);
void leerNHijos(void);
void leerRangoT(void);
void imprimirMatrizInicial(void);

pthread_barrier_t barrera;
float **matriz = NULL;
float **matrizTemp = NULL;

int nHilos = 0;
int inc = 0;
int filas = 0;
int columnas = 0;
int rangoT = 0;
int t = 0;

int main()
{   
    leerArchivo(FILE_NAME);
    leerNHijos();
    leerRangoT();

	int id = 0;
	pthread_t *pidhilos = NULL;
	struct DatoHilo *datohilo;

    pthread_barrier_init(&barrera, NULL, nHilos+1); // una barrera que espera a los hijos y al padre.
	
    //creacion de hilos
	pidhilos = (pthread_t *) calloc(nHilos, sizeof(pthread_t));

	for(; id<nHilos; id++)
    {	
	    datohilo = (struct DatoHilo*)malloc(sizeof(struct DatoHilo));
		datohilo->id = id;
        asignarRangos(id,&datohilo->F_ini,&datohilo->F_fin); //Asigno las filas de operacion.
		pthread_create(&pidhilos[id], NULL, TrabajoHilo, (void*)datohilo);
	}

    datohilo = (struct DatoHilo*)malloc(sizeof(struct DatoHilo));
    datohilo->id = id;
	TrabajoHilo((void*)datohilo);

    /*
        Nota: está de más ya que si el padre llega hasta acá es por que los hijos ya terminaron de hacer sus procesos.
            Sin embargo por buena practica hago que liberen memoria y salgan de la funcion.
    */
    for(int q=0; q<nHilos; q++)
		pthread_join(pidhilos[q], NULL);

	pthread_barrier_destroy(&barrera);
	free(pidhilos);
    printf("ha finalizado.");
	return 0;
}

void leerNHijos(void)
{
    while (1)
    {
       printf("\nNumero de hilos: ");
	   scanf("%d", &nHilos);
       if(nHilos <= 0)
            printf("\n-Error:El número ingresado debe ser positivo.\n");
       else if(filas % nHilos == 0 && nHilos > 0)
       {
           //Es un divisor
           inc = filas/nHilos;
           break;
       }
       else
            printf("\n-Error:El número ingresado (%d) no es un divisor del número de filas (%d).\n",nHilos,filas);
    }
}
void imprimirMatrizInicial(void)
{
    printf("\n\nResultado obtenido con t=0\n\n");
    for (size_t i = 0; i < filas; i++)
    {
        for (size_t q = 0; q < columnas; q++)
        {
            printf("%.2f ",matriz[i][q]);
        }
        printf("\n");
        
    }
    
}

void leerRangoT(void)
{
     while (1)
    {
       printf("\nNumero maximo de iteraciones para t: ");
	    scanf("%d", &rangoT);
       if(rangoT >= 0)
       {
           break;
       }
       printf("\n-Error:El número ingresado (%d) es negativo.\n",rangoT);
    }

}

void ErrorReport(char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void leerArchivo(char* name)
{
    FILE *file;
    file = fopen(name, "r");

    if (!file)
    {
        ErrorReport("Error al abrir el archivo de data");
    }

    fscanf(file, "%d %d", &filas,&columnas);

    if(filas < 3 || columnas < 3){
        ErrorReport("Lo sentimos, el archivo no se puede ejecutar. las filas y columnas deben ser > 3");
    }


    matriz = crearMatrizDinamica(filas,columnas);
    matrizTemp = crearMatrizDinamica(filas,columnas);

    for(size_t i = 0; i < filas; ++i)
    {
        for(size_t j = 0; j < columnas; ++j){
            fscanf(file, "%f", &matriz[i][j]);
        }
    }
    fclose (file); 
}

float **crearMatrizDinamica(int f,int c)
{
    float **matriz = (float**)calloc(f,sizeof(float**));
    if (!matriz)
    {
        ErrorReport("Error al asignar espacio a la matriz dinámica.");
    }

    for(int i = 0; i  < f; i++)
    {
        matriz[i] = (float*)calloc(c,sizeof(float*));
		
    }

    return matriz;  
}

void asignarRangos(int id,int* F_ini,int* F_fin)
{
    *F_fin = inc*(id + 1);
    *F_ini = inc*id;
}

void recorrerRejilla(int inicio,int fin)
{
    for (int i = inicio; i < fin; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            //printf("hilo %d modificando la matriz...\n",id);
            matrizTemp[i][j] = calcularValorPunto(i,j);
        }
    }
}

float calcularValorPunto(int x, int y)
{ 
    if(x == 0 || x == filas-1 || y == 0 || y == columnas-1)
    {
        return matriz[x][y];
    }
    else
    {
        return (matriz[x-1][y] + matriz[x+1][y] + matriz[x][y-1] + matriz[x][y+1])/4;
    }
    
}

void TrabajoHiloPrincipal(void)
{
    for(int x = 0; x < filas; x++)
    {
        for(int j = 0; j< columnas;j++)
        {
            matriz[x][j] = matrizTemp[x][j]; //Mientras imprime aprovecha los dos bucles para reescribir la matriz t_(n-1)
            printf("%.2f ",matriz[x][j]);
        }
        printf("\n"); 
    }
}

void * TrabajoHilo( void *arg)
{
    int id = ((struct DatoHilo *)arg)->id;
    int F_ini = ((struct DatoHilo *)arg)->F_ini;
    int F_fin = ((struct DatoHilo *)arg)->F_fin;

    if(id == nHilos)
    {
        imprimirMatrizInicial(); //Solo lo imprime el padre.
    }

	while(t < rangoT)
    {
        if(id != nHilos) //Instrucciones que ejecutaran los hilos que se crearon. es decir, excluye al principal.
        {
            recorrerRejilla(F_ini,F_fin);
        }

        //Barrera para esperar que los hilos terminen su trabajo.
        pthread_barrier_wait(&barrera);

        if(id == nHilos) //Trabajo del hilo principal.
        {
            printf("\n\nResultado obtenido con t=%d\n\n",t+1); 
            TrabajoHiloPrincipal();
            t++; //Aprovechando la exclusion, el hilo principal incrementa el valor de T.
        }
        //Los hijos esperan a que el padre termine.
        pthread_barrier_wait(&barrera); 
    }
    
	free(arg);

    if(id != nHilos)
	    pthread_exit(0);
}
