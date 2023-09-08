#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>

#include <wait.h>

#include <sys/shm.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <sys/ipc.h>

void mul_matrix(int ** a, int ** b, int ** c, int N, int i, int j) {

            c[i][j] = 0;
            for (int k = 0; k < N; k++)
                c[i][j] += a[i][k] * b[k][j];
        
}

void recorrer_anillo(int ** a, int ** b, int ** c, int l,int tam){

    //arriba
    for(int i=l;i<(tam-1);i++){
        mul_matrix(a,b,c,tam,l,i);
    }
    
    //derecha
    for(int i=l;i<(tam-l);i++){
        mul_matrix(a,b,c,tam,i,(tam-l-1));
    }
    
    //izquierda
    for(int i=l;i<tam-l;i++){
         mul_matrix(a,b,c,tam,i,l);
    }
    //abajo
    for(int i=l;i<(tam-l);i++){
        mul_matrix(a,b,c,tam,(tam-l-1),i);
    }

}



unsigned int sizeof_dm(int rows, int cols, size_t sizeElement) {
    size_t size = rows * sizeof(void * ); // index size
    size += (cols * rows * sizeElement); // Data size
    return size;
}

void create_index(void ** m, int rows, int cols, size_t sizeElement) {
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (int i = 1; i < rows; i++) {
        m[i] = (m[i - 1] + sizeRow);
    }
}

void print_matrix(int ** m, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%d\t", m[r][c]);
        }
        printf("\n");
    }
}

int main(int argc, char
    const * argv[]) {
    int shm_id, shm_id2, shm_id3, rows = 6, cols = 6, num_child = 0;
    pid_t pid_c;
    if (rows == cols ) { // es cuadrada 

        int ** m;
        int ** m2;
        int ** m3;

        int num_child = rows / 2;
        // num_child = num_child -1 ;

        size_t sizeMatrix = sizeof_dm(rows, cols, sizeof(int));
        shm_id = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
        shm_id2 = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
        shm_id3 = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
        //creacion de matrixs
        m = shmat(shm_id, NULL, 0);
        m2 = shmat(shm_id2, NULL, 0);
        m3 = shmat(shm_id3, NULL, 0);

        create_index((void * ) m, rows, cols, sizeof(int));
        create_index((void * ) m2, rows, cols, sizeof(int));
        create_index((void * ) m3, rows, cols, sizeof(int));

        //llenado la matriz a y b
        int num = 1;
        for (int f = 0; f < rows; f++) {
            for (int c = 0; c < cols; c++) {
                m[f][c] = num;
                m2[f][c] = num;
                num++;
            }
        }

        //creacion de los procesos

        for (int i = 0; i < num_child; i++) {
            pid_c = fork();
            if (pid_c == 0) { //hijos no salgan control creando mas hijos xd
                printf("\t\t\tProceso Hijo [%d] y fork [%d]\n", getpid(), i);

                // aki va la logica ojo   mul_matrix(m, m2, m3, rows, x , y);
                recorrer_anillo(m,m2,m3,i,rows);
                printf("\n");
                print_matrix(m3,rows,cols);
                printf("\n");
                //esto para tener acceso a la memoria compartida de las 3 matrices
               // sleep(2);
                //shmdt(m);
                //shmdt(m2);
                //shmdt(m3);

                exit(0); // break por eso no entra al final == 0 para no crear mas hijo de hijos 
            } else if (pid_c > 0) {
                wait(NULL); // los hijos se ejecutan primero
            } else {
                printf("supuesto error");
            }
        }

        //logica de los procesos
        
        if (pid_c > 0) { // padre
          //  wait(NULL); // padre no termine primero que el hijo

            printf("\t\t\tProceso Padre [%d]\n", getpid());

            //mostrando las matrices 
            //print_matrix(m, rows, cols);
            //printf("\n");
            //print_matrix(m2, rows, cols);
            printf("\n");

            //mostrando los resultados 
            printf("\n resultado multiplicacion \n");
            print_matrix(m3, rows, cols);
            printf("\n");

            // nose para que es esto xd
            shmdt(m);
            shmdt(m2);
            shmdt(m3);

            //esto es necesario para contorlar los procesos 
            shmctl(shm_id, IPC_RMID, 0);
            shmctl(shm_id2, IPC_RMID, 0);
            shmctl(shm_id3, IPC_RMID, 0);
        } else if (pid_c == 0) { // hijo nunca entrata por el exit(0) de arriba inncesario xd
            printf("\t\t\tProceso Hijo [%d] de 2 linea \n", getpid());
        } else { // solo por si un error sale
            printf("error fork no funciona");
        }

    } else {
        printf("matriz no cuadrada \n");
    }
    return 0;
}