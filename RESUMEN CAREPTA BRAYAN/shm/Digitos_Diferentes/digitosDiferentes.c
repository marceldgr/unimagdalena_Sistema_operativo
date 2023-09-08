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
//contar las lineas del archivo
int nLineasArchivo(){
    int r = 0;
    FILE *f = fopen("archivo.txt","r");
    char c;
    if( f ){
        c = fgetc(f);
        while( c != EOF){
            if( c == '\n' ) r++;
            c = fgetc(f);
        }
    }
    fclose(f);
    return r+1;
}
int digitosRepetidos (int num){
    int cant = 0;
    char vector[30];
    int k=0;
    while(num){
        int digito = num%10; // 9 % 10 = 9.
        vector[k] = digito + '0';
        k++;
        num /= 10; //9 / 10 = 0.234 
    }
    for(int x=0; x<strlen(vector); x++){
        for(int y=x+1; y <strlen(vector); y++){
            if(vector[x] == vector[y]) cant++;
        }
    }
    return cant;
}
int main()
{
    pid_t padre = getpid();
    int n = nLineasArchivo(),shmId,i;
    int *turno;
    int **matrix = NULL;
    size_t sizeMatrix = sizeof_dm(n,2,sizeof(int));
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    int shmId_t = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0600);
    
    turno = shmat(shmId_t,NULL,0);
    matrix = shmat(shmId,NULL,0);
    create_index((void*)matrix, n, 2, sizeof(int));
    *turno = 0;

    for( i = 0; i < n; i++) if(!fork())break;
    
    if (padre == getpid()) {
        FILE *f = fopen("archivo.txt","r");
        if(!f) exit(-1);
        int inicio, final;
        
        for(int  j = 0; j < n; j++)
        {
            fscanf(f,"%d",&inicio);
            fscanf(f,"%d",&final);
            matrix[j][0] = inicio;
            matrix[j][1] = final;
        }
        fclose(f);
        for( int r = 0; r < n; r++){
            for( int c = 0; c < 2; c++){
                printf("%d \t", matrix[r][c] );
            }
            printf("\n");
        }
        *turno = 1;
    }else{
        while(*turno == 0);
       
        int cant =0;
        int lim1 = matrix[i][0];
        int lim2 = matrix[i][1];
        
        for(int k = lim1; k <= lim2; k++)
        {
            if(digitosRepetidos(k) == 0) cant++;
        }
        printf("Hijo%d -> Cantidad de digitos no repetidos %d \n", i+1, cant);
    }
    return 0;
}
