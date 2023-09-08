#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
    double Rows = 5, Cols = 5;
    pid_t padre = getpid();
    /*printf("ingrese el numero de filas: \n");
    scanf("%f", &Rows);
    printf("ingrese el numero de columnas: \n");
    scanf("%f", &Cols);
    */
    int shmId;
    int shmId2;
    double *sum = NULL;
    double **matrix = NULL;
    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    float n = round(Cols/2);

    shmId2 = shmget(IPC_PRIVATE, sizeof(double)*n,IPC_CREAT | S_IRUSR | S_IWUSR);
    matrix = shmat(shmId,NULL,0);
    sum = shmat(shmId2,NULL,0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));

    int  i;
    for( i = 0; i < n; i++)
    {
        if(!fork()) break;
    }
    double valor = 0;
    for(int r = 0; r < Rows; r++)
    {
        for(int c = 0; c < Cols; c++)
        {
            matrix[r][c] = valor;
            valor++; 
        }
    }
    int irows =Rows, icols = Cols;
    if (getpid() != padre){
        sleep(1);
        double suma=0;
        for (int c=i; c<Cols-i; c++){
            suma += matrix[i][c];
            if (i != icols-i-1) suma += matrix[(irows-1)-i][c];
        }
        for (int f=i+1; f<(Rows-1)-i; f++){
            suma += matrix[f][i];
            suma += matrix[f][(icols-1)-i];
        }
        printf("i= %d, suma = %.1f \n", i,  suma);
        sum[i] = suma;
        shmdt(sum);
        shmdt(matrix);
    }else{
        double suma = 0;
        
        for (int a=0; a<n; a++) wait(NULL);
        
        for( int r = 0; r < Rows; r++){
            for( int c = 0; c < Cols; c++){
                printf("[%.1f] \t", matrix[r][c] );
            }
            printf("\n");
        }
        printf("\nla suma por anillos es:\n");
        for(int j = 0; j < n; j++){
            printf("[%.1f]\t",sum[j]);
            suma = suma + sum[j];
        }
        printf("\nla suma total es: %.1f\n",suma);

        double aux = 0;
        //ordenamiento por burbuja
        for( int k = 0; k < n; k++)
        {
            for( int r = 0; r < n-1; r++)
            { 
                if (sum[r]>sum[r+1]) {
                    aux = sum[r];
                    sum[r] = sum[r+1];
                    sum[r+1] = aux;
                }  
            }            
        }
        printf("el vector ordenado es:\n");
        for(int j = 0; j < n; j++){
            printf("[%.1f]\t",sum[j]); 
        }
        shmdt(sum);
        shmdt(matrix);
        shmctl(shmId,IPC_RMID,0);   
    }

    return 0;
}