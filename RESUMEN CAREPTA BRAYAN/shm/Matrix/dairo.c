#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int sizeof_dm(int rows,int cols,size_t sizeElement){
    size_t size = rows * sizeof(void*);
    size +=(cols* rows * sizeElement);
    return size;
}

void create_index(void **m,int rows, int cols,size_t sizeElement){
    int i;
    size_t sizeRow=cols * sizeElement;
    m[0]=m+rows;
    for(i=1;i<rows;i++){
        m[i]=(m[i-1]+sizeRow);
    }
}

int main()
{
    int Rows=15;
    int Cols=15;
    double **matrix=NULL;
    double shmId;
    int shm_size = 1024;
    size_t sizeMatrix = sizeof_dm(Rows, Cols, sizeof(double));
    shmId = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT | 0600);
    matrix = shmat(shmId,NULL,0);
    create_index((void*) matrix,Rows,Cols,sizeof(double));
    int x,y;
    
    for (x = 0; x < Rows; x++)
    {
        for (y = 0; y < Cols; y++)
        {
            matrix[x][y]=y+x;
        }
        printf("\n");
    }
    int c=fork();
    if(c==0){
        //sleep(3);
        printf("\n[%d]\n", getpid());
        for (x = 0; x < Rows; x++)
        {
            for (y = 0; y < Cols; y++)
            {
                //matrix[x][y] = y + x;
                printf("%.2f\t", matrix[x][y]);
            }
            printf("\n");
        }
        matrix[2][2]=777;

        shmdt(matrix);
        }
    else{
        wait(NULL);
        printf("\n[%d]\n", getpid());

        for (int x = 0; x < Rows; x++)
        {
            for (int y = 0; y < Cols; y++)
            {
                printf("%.2f\t", matrix[x][y]);
            }
            printf("\n");
        }
        shmdt(matrix);
        shmctl(shmId, IPC_RMID, 0);
        
    }

    return 0;
}