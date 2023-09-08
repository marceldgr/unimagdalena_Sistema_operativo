/*

Codigo hecho por:
	Daniel Miguel Vega Camargo - 2017214084

*/
#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>

unsigned int sizeof_dm(int, int, size_t);
void create_index(void **, int, int, size_t);
int calc_numChilds(int row);
int **crearMatrizDinamica(int);
void iniciarlizarMatriz(int**,int);
void mostrarMatriz(int** matriz,int n);
int multiplicacionMatrizXY(int **, int **, int , int ,int);
void recorrerAnillo(int **,int **matrizA,int **matrizB, int, int);
void imprimirRangos(int n,int cantChild);

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	size_t const sizeElement = sizeof(int);
	int n = 0; int i = 0; int shmId;//solo las filas ya que será cuadrada, tambien el tam de la col.
	
	while (n<2)
	{
		printf("(matriz nxn) Ingrese el valor de n:");
		scanf("%d",&n);
		if(n < 2)
		{
			printf("--Error: Ingresa un valor valido para N. (debe ser mayor que 1)\n");
		}
	}
	
	
	int cantChild = calc_numChilds(n);
	//Creo las matrices dinámicas A y B
	int **matrizA = crearMatrizDinamica(n);
	int **matrizB = crearMatrizDinamica(n);
	int **matrizC;

	//Inicializo las matrices A y B
	iniciarlizarMatriz(matrizA,n);
	iniciarlizarMatriz(matrizB,n);

	//Calculando el espacio necesario para almacenar a la matriz resultado en la memoria compartida.
	size_t sizeMatrizC = sizeof_dm(n, n, sizeElement);
	//creando el espacio de memoria.
	shmId = shmget(IPC_PRIVATE, sizeMatrizC, IPC_CREAT|0600);
	//Acoplando...
	matrizC = shmat(shmId, 0, 0);
	//Crear la matriz en la memoria compartida.
	create_index((void*) matrizC, n, n, sizeElement);
	//iniciarlizarMatriz(matrizC,n);
	printf("\n-- Se crearon %d hjos --\n",cantChild);
	printf("\n-- Rangos de los hijos --\n");
	imprimirRangos(n,cantChild);
	//recorrerAnillo(matrizC,matrizA, matrizB,  n,  1);
	printf("\n\n-- Historial de operaciones  --\n");
	for(;i < cantChild; i++)
	{
		if(!fork())break;
	}
	
	if(i != cantChild)
	{
		recorrerAnillo(matrizC,matrizA, matrizB,  n,  i);
		//desacoplo el segmento de memoria compartida.
		shmdt(matrizC);
	}
	else
	{
		for(int q = 0;q<cantChild;q++)
			wait(NULL);
		printf("\n\n-- matriz A --\n");
		mostrarMatriz(matrizA,n);
		printf("\n-- matriz B --\n");
		mostrarMatriz(matrizB,n);
		printf("\n-- matriz resultado --\n");
		mostrarMatriz(matrizC,n);
		//Desacoplo sel segmento de memoria compartida.
		shmdt(matrizC);
		//Elimino el espacio.
		shmctl(shmId, IPC_RMID, 0);
	}

	return EXIT_SUCCESS;
}

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement)
{
	size_t size = rows * sizeof(void *); //indexSize
	size += (cols * rows * sizeElement); //Data size
	return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
	int i;
	size_t sizeRow = cols * sizeElement;
	m[0] = m + (rows * sizeof(void *));
	for(i=1; i<rows; i++){
		m[i] = (m[i-1] + sizeRow);
	}
}

int calc_numChilds(int row){
	if(row%2!=0)
	{
		row = row+1;
	}
	return (int)row/2;
}

int **crearMatrizDinamica(int n)
{
    int **matriz = (int**)calloc(n,sizeof(int**));

    for(int i = 0; i  < n; i++)
    {
        matriz[i] = (int*)calloc(n,sizeof(int*));
		
    }

    return matriz;  
}

void iniciarlizarMatriz(int** matriz,int n )
{
	for (int i = 0; i < n; i++)
	{
		for (int q = 0; q < n;q++)
		{
			matriz[i][q] = (int)(rand() % 9)+1;
		}
	}
	
}

void mostrarMatriz(int** matriz,int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("[");
		for (int q = 0; q < n; q++)
		{
			if(q == n-1)
				printf("[%d]",matriz[i][q]);
			else
				printf("[%d],",matriz[i][q]);
		}
		printf("]\n");
	}
	
}
//IdxChild es igual al I del hijo.
void recorrerAnillo(int **matriz,int **matrizA,int **matrizB,int n, int idxChild)
{
	int cantChild = calc_numChilds(n);
	
	for(int j  = 0 ; j < n; j++) 
  	{	
		  //Fila sup e inf.
		if(j+idxChild < n-idxChild)
		{
			int x_sup = idxChild;		int y_sup = j+idxChild;
			int x_inf = n-1-idxChild; 	int y_inf = j+idxChild;

			matriz[x_sup][y_sup] = multiplicacionMatrizXY(matrizA,matrizB,n,x_sup,y_sup);//multiplicar part sup
			printf("\nEl hijo [%d] con pid [%d] modificó la matriz: [%d][%d] = [%d]",idxChild,getpid(),x_sup,y_sup,matriz[x_sup][y_sup]);
		
			if(n%2 == 0 || idxChild != cantChild -1)
			{
				matriz[x_inf][y_inf] = multiplicacionMatrizXY(matrizA,matrizB,n,x_inf,y_inf);//multiplicar part inf
				printf("\nEl hijo [%d] con pid [%d] modificó la matriz: [%d][%d] = [%d]",idxChild,getpid(),x_inf,y_inf,matriz[x_inf][y_inf]);
			}
			
		}
		 //col der e izq.
		if(idxChild != cantChild -1) //El ultimo uno lo requiere.
		{
			if(j+idxChild+1 < n-idxChild-1)
			{
				int x_izq = j+idxChild+1;	int y_izq = idxChild;
				int x_der = x_izq; 			int y_der = n-1-idxChild;
				matriz[x_izq][y_izq] = multiplicacionMatrizXY(matrizA,matrizB,n,x_izq,y_izq); //multiplicar part izq
				matriz[x_der][y_der] = multiplicacionMatrizXY(matrizA,matrizB,n,x_der,y_der); //multiplicar part der.
				printf("\nEl hijo [%d] con pid [%d] modificó la matriz: [%d][%d] = [%d]",idxChild,getpid(),x_izq,y_izq,matriz[x_izq][y_izq]);
				printf("\nEl hijo [%d] con pid [%d] modificó la matriz: [%d][%d] = [%d]",idxChild,getpid(),x_der,y_der,matriz[x_der][y_der]);
		 	}
		}
	}
	
}

int multiplicacionMatrizXY(int **matrizA, int **matrizB, int n, int x,int y)
{
	int suma = 0;

	for(int j = 0; j < n; j++)
	{
		suma += matrizA[x][j]*matrizB[j][y];
	}

	return suma;
}


void imprimirRangos(int n,int cantChild)
{
	for (int idxChild = 0; idxChild < cantChild; idxChild++)
	{
		printf("\nEl hijo [%d] va de [%d][%d] hasta [%d][%d] (lim sup)",idxChild,idxChild,idxChild,idxChild,n-idxChild-1);
		if(n%2 == 0 || idxChild != cantChild -1)
		{
			printf("\nEl hijo [%d] va de [%d][%d] hasta [%d][%d] (lim inf)",idxChild,n-idxChild-1,idxChild,n-idxChild-1,n-idxChild-1);
		}
		if(idxChild != cantChild -1) //El ultimo uno lo requiere.
		{
			printf("\nEl hijo [%d] va de [%d][%d] hasta [%d][%d] (lim izq)",idxChild,idxChild+1,idxChild,n-idxChild-2,idxChild);
			printf("\nEl hijo [%d] va de [%d][%d] hasta [%d][%d] (lim der)\n",idxChild,idxChild+1,n-idxChild-1,n-idxChild-2,n-idxChild-1);
		}
	}
	
	
}