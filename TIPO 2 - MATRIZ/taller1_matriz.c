#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * sizeof (void *); // index size
    size += (cols * rows * sizeElement); // Data size
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (int i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void print_matrix(double **m, int rows, int cols){
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            printf("%.2f\t", m[r][c]);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]){

    int shm_id, rows = 5, cols = 10;
    double **m;
    size_t sizeMatrix = sizeof_dm(rows, cols, sizeof(double));
    shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
    m = shmat(shm_id, NULL, 0);
    create_index((void*)m, rows, cols, sizeof(double));

    if(!fork()){
        sleep(2);
        printf("\t\t\tProceso Hijo [%d]\n", getpid());
        print_matrix(m, rows, cols);
        m[2][2] = -99.99;
        shmdt(m);
    }else{
        double num=0.0;
        for(int f=0; f<rows; f++){
            for(int c=0; c<cols; c++){
                m[f][c] = num;
                num++;
            }
        }
        wait(NULL);
        printf("\t\t\tProceso Padre [%d]\n", getpid());
        print_matrix(m, rows, cols);
        shmdt(m);
        shmctl(shm_id, IPC_RMID, 0);
    }


    return 0;
}
