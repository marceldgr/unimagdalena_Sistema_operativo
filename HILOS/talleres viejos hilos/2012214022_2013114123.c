#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct data{
    int n; //tamano de la matriz
    int id; //numero del hilo
}data;

pthread_t *vpt; //vector de hilos
int **mat1,**mat2,**mat3; //matrices globales
void * thread_handler_upp(void *); //manejador para la triangular superior
void * thread_handler_low(void *); //manejador para latriangular inferior
void * thread_handler_dia(void *); //manejador para la diagonal
int ** create_matrix(int ); //funcion para crear una matriz nxn de enteros dinamicamente
void fill_matrix_rand(int**,int ); //funcion para llenar la matriz con numeros aleatorios
void fill_matrix_empty(int**,int ); //funcion para llenar la matriz de valores vacios
void print_matrix(int**,int); //funcion para imprimir la matriz
void free_matrix_memory(int**, int); //funcion para liberar la memoria usada en una matriz generada dinamicamente
void operate_position(int,int,int);  //funcion para operar una posicion especifica

int main(){
    int i,n;
    srand(time(NULL));
    
    printf("Ingrese el tamaño de las matrices a utilizar: \n");
    scanf("%d", &n);
    
    mat1 = create_matrix(n); //crea la matriz 1
    fill_matrix_rand(mat1,n); //llena la matriz 1 con valores aleatorios de 0 a 9.
    mat2 = create_matrix(n); 
    fill_matrix_rand(mat2,n);
    mat3 = create_matrix(n);
    fill_matrix_empty(mat3,n); //llena la matriz 3 con ceros.
    
    printf("\nMatriz 1:\n");
    print_matrix(mat1,n);
    printf("Matriz 2:\n");
    print_matrix(mat2,n);
    
    data * vector_data[3]; 
    vpt = (pthread_t *) calloc(3, sizeof(pthread_t));
    
    void* (* handlers[3])(void*);
    handlers[0] = thread_handler_low;
    handlers[1] = thread_handler_upp;
    handlers[2] = thread_handler_dia;
    
    for(i=0; i<3; i++){
        vector_data[i] = (data *) malloc(sizeof(data));
        vector_data[i]->n=n;
        vector_data[i]->id=i;
        pthread_create(&vpt[i],NULL,handlers[i],vector_data[i]);
    }
    
    for(i=0; i<3; i++){
        pthread_join(vpt[i],NULL);
    }
    
    printf("\nMatriz 3 M1xM2 calculada: \n"); 
    print_matrix(mat3, n);
    
    free_matrix_memory(mat1,n); //libera la memoria utilizada.
    free_matrix_memory(mat2,n);
    free_matrix_memory(mat3,n);
    free(vpt);
    
    return 0;
}

void* thread_handler_upp(void * param){
    int n = ((data *) param)->n;
    int id= ((data *) param)->id;
    int i,j,init_j;
    printf("Hilo %d [%lu] calculando la triangular superior...\n", id, (long unsigned) pthread_self());
    init_j=1;
    for(i=0; i<n-1; i++){
        for(j=init_j; j<n; j++){
            operate_position(n, i, j);
        }
        init_j++;
    }
}


void * thread_handler_low(void* param){
    int n = ((data *) param)->n;
    int id= ((data *) param)->id;
    int i, j, lim_j;
    printf("Hilo %d [%lu] calculando la triangular inferior...\n", id, (long unsigned) pthread_self());
    lim_j=1;
    for(i=1; i<n; i++){
        for(j=0; j<lim_j; j++){
            operate_position(n, i, j);
        }
        lim_j++;
    }

}

void * thread_handler_dia(void* param){
    int n = ((data *) param)->n;
    int id= ((data *) param)->id;
    int i, init_j;
    printf("Hilo %d [%lu] calculando la diagonal...\n", id, (long unsigned) pthread_self());
    for(i=0; i<n; i++){
        operate_position(n, i, i);
    }
}

void fill_matrix_empty(int **mat,int n){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            mat[i][j]= 0;
        }
    }
}

int** create_matrix(int n){
    int i;
    int ** matrix;
    matrix = (int **)  calloc(n, sizeof(int*));
    for(i=0; i<n; i++){
        matrix[i] = (int *)  calloc(n, sizeof(int*));
    }
    return matrix;
}

void fill_matrix_rand(int **mat,int n){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            mat[i][j]= rand()%10;
        }
    }
    
}

void print_matrix(int **mat,int n){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            printf("[%d]\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void free_matrix_memory(int **mat,int n){
    int i;
    for(i=0; i<n; i++){
        free(mat[i]); //libera las filas
    }
    free(mat); //libera la memoria de la matriz
}

void operate_position(int n,int i, int j){
    int p;
    for(p=0; p<n; p++){ //opera una posicion especifica en la matriz 3.
        mat3[i][j] += mat1[i][p]*mat2[p][j]; 
    }
}