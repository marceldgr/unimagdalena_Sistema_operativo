#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
      
    for( i = 1; i < rows; i++)
    {
        m[i] = (m[i-1] + sizeRow);
    }
}
unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * sizeof(void *);// indexSize
    size += (cols * rows *sizeElement);// datasize
    return size;
}

int main(int argc, char const *argv[])
{
    int Rows = 5, Cols = 5;
    pid_t  padre = getpid();
    int shmId;
    int **m = NULL;
    int *sum = NULL;
    int shmId2=shmget(IPC_PRIVATE,sizeof(int)*Rows,IPC_CREAT | S_IRUSR | S_IWUSR);
	sum=shmat(shmId2,0,0);
    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(int));
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    m = shmat(shmId,NULL,0);
    create_index((void*)m, Rows, Cols, sizeof(int));

    int r, c, i;
    
    for( r = 0; r < Rows; r++)
    {
        for( c = 0; c < Cols; c++)
        {
            m[r][c] = r+c; 
        }
    }
      
    for( i = 0; i < Cols; i++)
    {
        if (!fork()) break;    
    }
    
    if(padre == getpid()){
        int suma = 0;
        for( c = 0; c < Cols; c++)
        {
            wait(NULL);
        }
        printf("\nla suma por columnas es:\n");
        for( r = 0; r < Rows; r++)
        {
            printf("[%d] \t",sum[r]);
            suma = suma + sum[r];
        }
        printf("\n[%d] la suma total es: %d\n",getpid(),suma);
        shmdt(m);
        shmdt(sum);
        shmctl(shmId,IPC_RMID,0);
        shmctl(shmId2,IPC_RMID,0);
    }else{
        if(i == 0){
            sleep(1);
            
            for( r = 0; r < Rows; r++)
            {
                for( c = 0; c < Cols; c++)
                {
                printf("[%d] \t", m[r][c] );
                }
                printf("\n");
            }                      

        }
        int aux = 0;        
        for( c = 0; c < Cols; c++)
        {
            aux = aux + m[c][i];
        }
        sum[i] = aux;
        shmdt(sum);
        shmdt(m);
    }

    return 0;
}
