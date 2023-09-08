#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>


// Mis funciones
unsigned int sizeof_dm(int rows, int col, size_t sizeElement);
void create_index(void **m, int rows, int cols, size_t sizeElement);  
void mostrar(double **m, int rows, int cols);


int main(int argc, char const *argv[]){

    int shmId;
   
    double **m;
    int i,rows = 5,cols = 10;

    size_t sizeMatrix = sizeof_dm(rows, cols, sizeof(double));// tamaño de la estructura
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0666);// creacion de la memoria
    m = shmat(shmId, NULL, 0);// acoplando memoria
    create_index((void *)m, rows, cols, sizeof(double));


    if(!fork()){ 
        usleep(1000);
        printf("\n Impresion: Proc. Hijo");
        mostrar((double **)m, rows, cols);
        m[2][2] = -99.99;

        shmdt(m); 
    }else{

            int x, y;
            double cont = 0;
            for(x = 0; x < rows; x++){
                printf("\n");
                for(y = 0; y < cols; y++){
                    m[x][y] = cont;
                    cont++;
                
                }

            }
        
        wait(NULL);
        
        printf("\n Impresion: Proc. padre");
        mostrar((double **)m, rows, cols);
            
        shmdt(m);// desacloplando memoria
        shmctl(shmId, IPC_RMID, 0);}// eliminando memoria compartida

 

    printf("\n\n");
    return 0;
}



unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
    size_t size = rows * sizeof(void *); // IndexSize
    size += (col * rows)* sizeElement;    //rows - size

    return size;   
}

void create_index(void **m, int rows, int cols, size_t sizeElement){  
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m+rows;
    for(i = 1; i < rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void mostrar(double **m, int rows, int cols){
    int x, y;
    for(x = 0; x < rows; x++){
        printf("\n");
        for(y = 0; y < cols; y++){
            printf(" [ %.2f ] ", m[x][y]);                
         }

    }



}



