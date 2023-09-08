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
int calc_numChilds(int);
void AgregarValoresRandom(int**,int);
void mostrarMatriz(int** matriz,int nxn);
int calcCoordM(int **, int **, int , int ,int);
void recorrerAnillo(int **,int **,int **, int, int);
void imprimirRangos(int nxn,int cantChild);

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	size_t const sizeElement = sizeof(int);
	int n = 0; 
	int i = 0; 
	int shmId;
	
	
	//Ya verás como validas.
	while (n<2)
	{
		printf("Ingrese el valor de n (nxn):");
		scanf("%d",&n);
		if(n < 2)
		{
			printf("--Error: Ingresa un valor valido para N. (debe ser mayor que 1)\n");
		}
	}
	
	
	int cantChild = calc_numChilds(n);
	//Creo las matrices dinámicas A y B
	int **matrizA = (int**)calloc(n,sizeof(int**));

    for(int i = 0; i  < n; i++)
    {
        matrizA[i] = (int*)calloc(n,sizeof(int*));
    }

	AgregarValoresRandom(matrizA,n);

	int **matrizB = (int**)calloc(n,sizeof(int**));
	for(int i = 0; i  < n; i++)
    {
        matrizB[i] = (int*)calloc(n,sizeof(int*));
    }
	AgregarValoresRandom(matrizB,n);
	int **matrizResul;

	//Inicializo las matrices A y B
	
	


	size_t sizematrizResul = sizeof_dm(n, n, sizeElement);
	shmId = shmget(IPC_PRIVATE, sizematrizResul, IPC_CREAT|0600);
	matrizResul = shmat(shmId, 0, 0);
	create_index((void*) matrizResul, n, n, sizeElement);
	
	printf("\n\n Calculo: \n");
	pid_t pidchild;
	for(;i < cantChild; i++)
	{	
		pidchild = fork();
		if(!pidchild)break;
	}
	switch (pidchild)
	{
		case -1:
			perror("Error fork"); 
            exit(EXIT_FAILURE);
		break;
		case 0: //hijo
			recorrerAnillo(matrizResul,matrizA, matrizB,  n,  i);
			shmdt(matrizResul);

		break;
	
		default://hijo
			for(int q = 0;q<cantChild;q++)
						wait(NULL);
					printf("\n\nmatriz A:\n");
					mostrarMatriz(matrizA,n);
					printf("\nmatriz B:\n");
					mostrarMatriz(matrizB,n);
					printf("\nmatriz resultado:\n");
					mostrarMatriz(matrizResul,n);
					shmdt(matrizResul);
					shmctl(shmId, IPC_RMID, 0);
					break;
			
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

int calc_numChilds(int nxn)
{
	if(nxn%2!=0)
	{
		nxn = nxn+1;
	}
	return nxn/2;
}

void AgregarValoresRandom(int** matriz,int nxn )
{
	for (int i = 0; i < nxn; i++)
	{
		for (int q = 0; q < nxn;q++)
		{
			matriz[i][q] = (int)(rand() % 7);
		}
	}
	
}

void mostrarMatriz(int** matriz,int nxn)
{
	for (int i = 0; i < nxn; i++)
	{
		for (int q = 0; q < nxn; q++)
		{
			printf("%d  ",matriz[i][q]);
		}
		printf("\n");
	}
	
}
//index es igual al I del hijo.
void recorrerAnillo(int **matriz,int **matrizA,int **matrizB,int n, int index)
{
	int cantChild = calc_numChilds(n);
	
	for(int i  = 0 ; i < n; i++) 
  	{	
		if(i+index < n-index)
		{
			matriz[index][i+index] = calcCoordM(matrizA,matrizB,n,index,i+index);//multiplicar part sup
			printf("\nEl hijo [%d] calculó la posicion [%d][%d] = %d",getpid(),index,i+index,matriz[index][i+index]);	
			if(n%2 == 0 || index != cantChild -1)
			{
				matriz[n-1-index][i+index] = calcCoordM(matrizA,matrizB,n,n-1-index,i+index);//multiplicar part inf
				printf("\nEl hijo [%d] calculó la posicion [%d][%d] = %d",getpid(),n-1-index,i+index,matriz[n-1-index][i+index]);
			}		
		}
		if(index != cantChild -1) 
		{
			if(i+index+1 < n-index-1)
			{
				matriz[i+index+1][index] = calcCoordM(matrizA,matrizB,n,i+index+1,index); //multiplicar part izq
				printf("\nEl hijo [%d] calculó la posicion [%d][%d] = %d",getpid(),i+index+1,index,matriz[i+index+1][index]);
				matriz[i+index+1][n-1-index] = calcCoordM(matrizA,matrizB,n,i+index+1,n-1-index); //multiplicar part der.
				printf("\nEl hijo [%d] calculó la posicion [%d][%d] = %d",getpid(),i+index+1,n-1-index,matriz[i+index+1][n-1-index]);
		 	}
		}
	}
	
}
//No sé si quieras poner esos for ahí.
int calcCoordM(int **matrizA, int **matrizB, int n, int x,int y)
{
	int suma = 0;

	for(int i = 0; i < n; i++)
	{
		suma += matrizA[x][i]*matrizB[i][y];
	}

	return suma;
}


