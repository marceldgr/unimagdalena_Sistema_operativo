#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
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
    double Rows = 5, Cols = 10;
    double shmId;
    double **matrix = NULL;

    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    matrix = shmat(shmId,NULL,0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));
    int r, c;
    
    for( r = 0; r < Rows; r++)
    {
        for( c = 0; c < Cols; c++)
        {
            matrix[r][c] = r + c;
        }  
        
    }
    
    if (!fork()) {
        sleep(1);   
        for( r = 0; r < Rows; r++)
        {
            for( c = 0; c < Cols; c++)
            {
                printf("%.2f\t",matrix[r][c]);
            }
            printf("\n");
        }
        matrix[2][2] = -99;
        shmdt(matrix);
    }else{
        wait(NULL);   
         printf("\n[%d]\n", getpid());
        for( r = 0; r < Rows; r++)
        {   
            for( c = 0; c < Cols; c++)
            {
                printf("%.2f\t",matrix[r][c]);
            }
            printf("\n");
        }
        shmdt(matrix);
        shmctl(shmId,IPC_RMID,0);    
    }
    
    return 0;   
}