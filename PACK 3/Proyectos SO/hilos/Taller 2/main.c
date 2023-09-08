/*

Taller de hilos matrices diagonales.

Elaborado por:
    Daniel Miguel Vega Camargo:    2017214084 | Grupo 1
    Luis Armando Zuñiga Paredes:   2015114089 | Grupo 2

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define FILE_NAME "archivo.txt"
#define N_HILOS 2

struct Encontrada
{
    int id_hilo;
    int posicion_x,posicion_y;
};

struct Palabra
{
    char  *palabra;
    int longitud;
    struct Encontrada *encontrada;
}*palabra;

void leerArchivo(char*);
char **crearMatrizDinamica(int f,int c);
void ErrorReport(char*);
void * TrabajoHilo(void *);
int recorrerDiagonales(int ,int , struct Palabra *,int , int);
void buscarPalabraPuntoXY(int , int , int );
void marcarPalabra(int id_hilo, int id_palabra, int x, int y);

int filas,columnas;
char **tablero;
int cantPalabras;
pthread_mutex_t mutex;
//Palabras_restantes nos ayudará a evitar que los hilos no sigan recorriendo la matriz si no es
//necesario, es decir, si ya no quedan más palabras que buscar.
int Palabras_restates;

int main()
{
    clock_t t_ini, t_fin;
    double secs;

    leerArchivo(FILE_NAME);

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        ErrorReport("Error al inicializar el mutex.");
    }

    Palabras_restates = cantPalabras; 
    int id = 0;
	pthread_t *pidhilos = NULL;
	
    //creacion de hilos
	pidhilos = (pthread_t *) calloc(N_HILOS, sizeof(pthread_t));

	for(; id < N_HILOS; id++)
    {	
        int * f = malloc(sizeof(int));
        *f = id;
		pthread_create(&pidhilos[id], NULL, TrabajoHilo, f);
	}
    
    for(int q=0; q < N_HILOS; q++)
		pthread_join(pidhilos[q], NULL);
	
    for (int i = 0; i < cantPalabras; i++)
    {

         if(palabra[i].encontrada != NULL)
         {
            printf("\n %s: en posicion inicial %d-%d",palabra[i].palabra,palabra[i].encontrada->posicion_x+1,palabra[i].encontrada->posicion_y+1);
            if(palabra[i].encontrada->id_hilo == 0)
            {
                printf(" (hilo arriba-abajo)\n");
            }
            else
            {
                printf(" (hilo abajo-arriba)\n");
            }
         }
         else
         {
             printf("\n %s: Palabra no encontrada.\n",palabra[i].palabra);
         }
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

void leerArchivo(char* name)
{
    FILE *file;
    file = fopen(name, "r");

    if (!file)
    {
        ErrorReport("Error al abrir el archivo que contiene el tablero.");
    }

    fscanf(file, "%d", &filas);
    fscanf(file, "%d", &columnas);

    tablero = crearMatrizDinamica(filas,columnas);

    for(int i = 0; i < filas; ++i)
    {
        for(int j = 0; j < columnas; ++j){
            fscanf(file, " %c ", &tablero[i][j]);
        }
    }

    fscanf(file, "%d", &cantPalabras);
    palabra = (struct Palabra*)calloc(cantPalabras,sizeof(struct Palabra));

    char buffer[50];
    int longitud = 0;
    for (int i = 0; i < cantPalabras; i++)
    {
        fscanf(file, "%s", buffer);
        longitud = strnlen(buffer, 50);
        palabra[i].palabra = (char*)calloc(longitud,sizeof(char));
        strcpy(palabra[i].palabra, buffer);
        palabra[i].encontrada = NULL;
        palabra[i].longitud = longitud;
    }
    
    fclose (file); 
}

char **crearMatrizDinamica(int f,int c)
{
    char **matriz = (char**)calloc(f,sizeof(char**));
    if (!matriz)
    {
        ErrorReport("Error al asignar espacio a la matriz dinámica.");
    }

    for(int i = 0; i  < f; i++)
    {
        matriz[i] = (char*)calloc(c,sizeof(char*));
		
    }

    return matriz;  
}


void * TrabajoHilo( void *arg)
{
    int id = *((int *) arg);
    clock_t t_ini, t_fin;
    double secs;

    t_ini = clock();
    
    if(id == 0)
    {   //Para agilizar el proceso y evitar iteraciones de más
        //Palabras_restantes es una marca que evita que los hilos
        //sigan explorando el tablero en busca de palabras.
        for(int x = 0; x < filas && Palabras_restates != 0 ; x++)
            for(int y = 0; y < columnas && Palabras_restates != 0; y++)
            {
                buscarPalabraPuntoXY(x,y,id);
            }
    }
    else
    {//abajo arriba.

        for(int x = filas -1 ; x >= 0 && Palabras_restates != 0; x--)
            for(int y = columnas -1; y >= 0 && Palabras_restates != 0; y--)
            {
                buscarPalabraPuntoXY(x,y,id);
            }
    }
    
    t_fin = clock();
    secs = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
    printf("\nTiempo de trabajo del hilo con pid [%ld] fue de: %.16g milisegundos\n", pthread_self(),secs * 1000.0);
	pthread_exit(0);
}

void buscarPalabraPuntoXY(int x, int y , int id_hilo)
{  
    struct Palabra *aux;

    for (int i = 0; i < cantPalabras ; i++)
    {
        aux = &palabra[i];
        if(aux->encontrada == NULL)
        {
            if(tablero[x][y] == aux->palabra[0])
            {
                //Recorre las diagonales para ver si encuenta la palabra
                int encontrada = recorrerDiagonales(x,y,aux,id_hilo,0); 
                if(encontrada)
                {
                    //si la encuentra procede a marcarla.
                    marcarPalabra(id_hilo, i, x, y);
                }
            }
        }
    }
}

int recorrerDiagonales(int x,int y, struct Palabra *aux, int id_hijo ,int invertir_palabra)
{
    int contador = 1;  //número de aciertos.
    //Antes de recorrer y ver si la la palabra se encuentra en ese punto estaré validando antes si
    //en esa posicion es posible encontrar esa palabra;
    int y_der = y + (aux->longitud -1) <= (columnas -1);
    int y_izq = y >= (aux->longitud -1);
    int x_abajo = x + (aux->longitud -1) <= (filas -1);
    int x_arriba = x >= (aux->longitud-1);

    char *palabra = (char*)calloc(aux->longitud,sizeof(char));
    strcpy(palabra,aux->palabra);

    //Se evalua si se puede mover hacia la derecha
    if(y_der)
    {
        //al hilo arriba abajo le interesan las diagonales inferiores.
        if(x_abajo && id_hijo == 0)
        {
            /* 
                Ejemplo de lo que hace.
                    x x x x x
                    x x x x x
                    x x * x x
                    x x x * x
                    x x x x *
            */
            for(int j = 1; j < aux->longitud; j++)
            {
                if(tablero[x+j][y+j] == palabra[j])
                {
                    contador++;
                }
                else
                {
                    contador = 1;
                    break;
                }
            }
            if(contador == aux->longitud){
                return 1;
            }
        }
        //al hilo abajo arriba le interesan las diagonales superiores
        if(x_arriba && id_hijo == 1)
        {
            /* 
                Ejemplo de lo que hace.
                x x x x *
                x x x * x
                x x * x x
                x x x x x
                x x x x x
            */
            for(int j = 1; j < aux->longitud; j++)
            {
                if(tablero[x-j][y+j] == palabra[j])
                {
                    contador++;
                }
                else{
                    contador = 1;
                    break;
                }
            }
            if(contador == aux->longitud){
                return 1;
            }
            
        }
    }
    //Se evalua si se puede mover hacia la izquierda
    if(y_izq)
    {
        
        if(x_abajo && id_hijo == 0)
        {
            /* 
                Ejemplo de lo que hace.
                x x x x x
                x x x x x
                x x * x x
                x * x x x
                * x x x x
            */
            for(int j = 1; j < aux->longitud; j++)
            {
                if(tablero[x+j][y-j] == palabra[j])
                {
                    contador++;
                }
                else{
                    contador = 1;
                    break;
                }
            }
            if(contador == aux->longitud){
                return 1;
            }
        }
        if(x_arriba && id_hijo == 1)
        {
             /* 
                Ejemplo de lo que hace.
                * x x x x
                x * x x x
                x x * x x
                x x x x x
                x x x x x
            */
            for(int j = 1; j < aux->longitud; j++)
            {
                if(tablero[x-j][y-j] == palabra[j])
                {
                    contador++;
                }
                else
                {
                    contador = 1;
                    break;
                }
            }
            if(contador == aux->longitud){
                return 1;
            }
        }
    }

    return 0;

}

void marcarPalabra(int id_hilo, int id_palabra, int x, int y)
{
    pthread_mutex_lock(&mutex);
    //En el peor de los casos los hilos piden el mutex al tiempo. 
    //la verificacion se hace para verificar si despues de la espera por el mutex la palabra
    //no ha sido marcada.
    if(palabra[id_palabra].encontrada == NULL)
    {
        struct Encontrada* encontrada = (struct Encontrada *)malloc(sizeof(struct Encontrada));
        encontrada->id_hilo = id_hilo;
        encontrada->posicion_x = x;
        encontrada->posicion_y = y;
        palabra[id_palabra].encontrada = encontrada;
        //printf("encontrado por el hilo %d\n", encontrada->id_hilo); 
        Palabras_restates -= 1; //resta 1 a las palabras restantes para que pueda salir de la busqueda.
    }
    pthread_mutex_unlock(&mutex);
}

void ErrorReport(char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
