//
//  main.c
//  Snail Matrix
//
//  Created by Cristian Boada on 12/07/21.
//  Copyright © 2021 Cristian Boada. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <math.h>


const int S = 18;
//const double A[S][S] = {
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//};
//const double B[S][S] = {
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//    {1, 2, 3, 4, 5, 6},
//};

unsigned int sizeof_dm(int, size_t);
void create_index(void **, int, size_t);
void rand_matrix(double matrix[][S], int);
void init_matrix(double **matrix, int);
void print_matrix(double **matrix, int);
void process_matrix(double **matrix, double A[][S], double B[][S], int pos, int pid);
void error(char *msg);

int main(int argc, const char * argv[]) {
    // insert code here...
    const int N_PROCESS = ceil((double)S/2);
    int shmId;
    double **matrix;
    double A[S][S];
    double B[S][S];
    
    size_t sizeMatrix = sizeof_dm(S, sizeof(double));
    shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    matrix = shmat(shmId, NULL, 0);
    create_index((void *)matrix, S, sizeof(double));
    
    int state;
    pid_t pids[N_PROCESS];
    pid_t pid = getpid();
    
    init_matrix(matrix, S);
    rand_matrix(A, S);
    rand_matrix(B, S);
    
    for(int i=0; i<N_PROCESS; i++){
        pid = fork();
        
        if (pid < 0) {
            error("Error fork");
        }
        
        if (!pid) {
            printf("Hello child process: %d created\n", getpid());
            process_matrix(matrix, A, B, i, getpid());
            shmdt(matrix);
            break;
        } else {
            pids[i] = pid;
        }
        
    }
    
    if (pid > 0) {
        for (int i=0; i<N_PROCESS; i++) {
            printf("Waiting for %d process to finish\n", pids[i]);
            waitpid(pids[i], &state, 0);
            printf("Process %d finished\n", pids[i]);
        }
        
        printf("Hello father process: %d created\n", getpid());
        print_matrix(matrix, S);
        
        shmdt(matrix);
        shmctl(shmId, IPC_RMID, 0);
    }
    
    return EXIT_SUCCESS;
}

unsigned int sizeof_dm(int s, size_t sizeElement){
    size_t size = s * sizeof(double *); //indexSize
    size+= (s * s * sizeElement); //Data size
    return size;
}
void create_index(void **m, int s, size_t sizeElement) {
    int i;
    size_t sizeRow = s * sizeElement;
    m[0] = m + (s * sizeof(void *));
    for(i=1; i<s; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void rand_matrix(double matrix[][S], int s) {
    int cont = 1;
    for (int i=0; i<s; i++) {
        for (int j=0; j<s; j++) {
            matrix[i][j] = cont;
            cont = cont + 1;
        }
        cont = 1;
    }
}
void init_matrix(double **matrix, int s) {
    for (int i=0; i<s; i++) {
        for (int j=0; j<s; j++) {
            matrix[i][j] = 0;
        }
    }
}
void print_matrix(double **matrix, int s) {
    for (int i=0; i<s; i++) {
        for (int j=0; j<s; j++) {
            printf("[%.f]", matrix[i][j]);
        }
        printf("\n");
    }
}
void process_matrix(double **matrix, double A[][S], double B[][S], int pos, int pid) {
    int l = S-pos;
    for (int i=pos; i<l; i++) {
        for (int j=pos; j<l; j++) {
            if ((i==pos || j==pos) || (i==l-1 || j==l-1)){
                //printf("[%.f]", A[i][j]);
                for (int k=0; k<S; k++) {
                    matrix[i][j] += A[i][k] * B[k][j];
                }
                printf("\nProcess[%d][%d] write to position [%d][%d] with value: %.f", pos, pid, i, j, matrix[i][j]);
            } else {
                printf("   ");
            }
            
        }
        printf("\n");
    }
}

void error(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
