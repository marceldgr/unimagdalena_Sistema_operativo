#include<stdio.h>
#include<unistd.h> // libreria para fork
//#include<string.h>
//#include<stdlib.h>
#include<wait.h> // libreria para wait
//#include<sys/types.h>
#include<sys/shm.h> // libreria para memoria compartida
//#include<sys/stat.h>
#include<signal.h> // libreria para señales
void manejador(){ }
//pid_t pid;
int shmId;
int Rows; // numero de filas
int Cols; // numero de columnas
int j,k,c;
double **matrix;


unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
	size_t size = rows * sizeof(void *);
	size += (col*sizeElement);
	return size;
}

void create_index(void**m, int rows, int cols, size_t sizeElement){
	int i;
	size_t sizerows = cols*sizeElement;
	m[0] = m+rows;
	for(i=1; i<cols; ++i){
		m[i] = (m[i-1]+sizerows);
		}
}

int main(){
	signal(SIGUSR1,manejador); //invoca el manejador de la señal
	
				printf("Ingrese el tamano del columnas ");
				scanf("%d",&Cols);
				
				printf("Ingrese el tamano del filas ");
				scanf("%d",&Rows);
				
				size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
				shmId = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
				matrix=shmat(shmId, NULL, 0);
				create_index((void*)matrix,Rows,Cols,sizeof(double));
					
				if(!(c=fork())){ //hijo
			
				pause(); // espera la señal del padre
				printf("hijo\n");
				
				for(k=0; k<Cols; k++){
						for(int j=0; j<Rows; j++){
								matrix[k][j]=(double)j/(double)(j+1);
							}
						}
				usleep(2500);
				for(k=0; k<Cols; k++){
					for(int j=0; j<Rows; j++){
						printf("[%f]",matrix[k][j] );
						//printf("valor en col %d fila %d = %.6lf\n",k,j ,matrix[k][j]);
							}
					printf("\n");					
				}
				usleep(1000);
				kill(getppid(),SIGUSR1);
				}
			else{ //padre
				printf("padre\n");
				usleep(1000);			
				kill(c,SIGUSR1); // envia la señal al hijo
				pause(); // espera la señal
				shmdt(matrix);
				shmctl(shmId,IPC_RMID,0);
				}
return 0;	
}
