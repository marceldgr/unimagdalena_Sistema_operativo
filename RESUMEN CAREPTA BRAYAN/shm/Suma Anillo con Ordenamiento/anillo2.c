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
    double Rows = 4, Cols = 4;
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
    double n = round(Cols/2);

    shmId2 = shmget(IPC_PRIVATE, sizeof(double)*n,IPC_CREAT | S_IRUSR | S_IWUSR);
    matrix = shmat(shmId,NULL,0);
    sum = shmat(shmId2,NULL,0);
    create_index((void*)matrix, Rows, Cols, sizeof(double));

    double valor = 0;
    for(int j = 0; j < Rows; j++)
    {        
        for( int c = 0; c < Cols; c++)
        {
            matrix[j][c] = valor;
            valor++;
        }        
    }
    int i; 
    
    for( i = 0; i < n; i++)
    {
        if(!fork()) break;
    }
    
    if(getpid() != padre){
        sleep(4);
        double suma = 0;
        int par = 0, par2 = 0, par3 = 0, par4 = 0;
        int icols = Cols, irows = Rows;
        printf("\n");
        for(int  c = i; c <icols-i ; c++)
        {
           suma += matrix[i][c];
           par = matrix[i][c];
           if(i != (irows-i)-1) {
               suma += matrix[(irows-i)-1][c];
               par2 = matrix[(irows-i)-1][c];
           }
           if( par%2 == 0 && par2%2 == 0){
               printf("[%d],[%d],",par,par2);
           }
        }
        
        for(int r = i+1; r <(irows-i)-1 ; r++)
        {
            suma += matrix[r][i];
            par3 = matrix[r][i];
            if(i != (icols-i)-1){
                suma += matrix[r][(icols-i)-1];                
                par4 = matrix[r][(icols-i)-1];
            }
            if( par3%2 == 0 && par4%2 == 0){
               printf("[%d],[%d],",par3,par4);
           }
        }
        printf("\n");
      //  printf("\ni = %d la suma es: %.0f\n",i,suma);
        sum[i] = suma;
        shmdt(sum);
        shmdt(matrix);
    }else{
        for (int a=0; a<n; a++) wait(NULL);
        for(int j = 0; j < Rows; j++)
        {        
            for( int c = 0; c < Cols; c++)
            {
               printf("[%.0f] \t",matrix[j][c]); 
            }  
            printf("\n");
        }
        printf("el vector de resultados es: \n");
        double suma = 0;
        for( int s = 0; s < n; s++)
        {
            suma += sum[s];
            printf("[%.0f]",sum[s]);
        }        
        printf("\nla suma es: %.0f \n ", suma);
        shmdt(sum);
        shmdt(matrix);
        shmctl(shmId,IPC_RMID,0); 
    }
    
    return 0;
}