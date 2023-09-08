#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>


unsigned int sizeof_dm(int rows,int col,size_t sizeElement);
void create_index(void **m,int rows,int cols,size_t sizeElement); 

int main(){
	int i;
	int rows=3,cols=3;
	pid_t padre=getpid();
	int **m;
	int *sum;
	int shmId2=shmget(IPC_PRIVATE,sizeof(int)*rows,IPC_CREAT | S_IRUSR | S_IWUSR);
	sum=shmat(shmId2,0,0);

	size_t sizeMatrix=sizeof_dm(rows,cols,sizeof(int));
	int shmId=shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT | S_IRUSR | S_IWUSR);
	m=shmat(shmId,0,0);
	create_index((void *)m,rows,cols,sizeof(int));
	int valor=0;
	for(int k=0;k<rows;k++){
			for(int j=0;j<cols;j++){
				m[k][j]=valor;
				valor++;
			}
		}

	for(i=0;i<cols;i++){
		if(fork()==0){break;}
	}

	if(padre==getpid()){
		
		int sumaF=0;
		printf("padre\n");
		for(int k=0;k<cols;k++){
				wait(NULL);
		}

		for(int k=0;k<rows;k++){
			printf("[%d]",sum[k] );
			sumaF=sumaF+sum[k];
		}
		printf("\nEl resultado de las final de las sumas de la matriz: %d\n",sumaF);
		shmdt(sum);
		shmdt(m);
		shmctl(shmId2, IPC_RMID, 0);
		shmctl(shmId, IPC_RMID, 0);
	}else{
		int acu=0;
		if(i==0){
			for(int k=0;k<rows;k++){
				for(int j=0;j<cols;j++){
					printf("[%d]",m[k][j]);
				}
			printf("\n");
		    }
		printf("\n");
		}
		for(int s=0;s<rows;s++){
			acu=acu+m[s][i];
		}
		sum[i]=acu;
		shmdt(sum);
		shmdt(m);
	}

	return 0;

}

unsigned int sizeof_dm(int rows,int col,size_t sizeElement){
	size_t size=rows*sizeof(void *); //indexSize
	size +=(col*rows*sizeElement);//rows - size
	return size;
}

void create_index(void **m,int rows,int cols,size_t sizeElement){
	int i;
	size_t sizeRow=cols*sizeElement;
	m[0]=m+rows;
	for (int i = 1; i < rows; i++){
		m[i]=(m[i-1]+sizeRow);
	}
}