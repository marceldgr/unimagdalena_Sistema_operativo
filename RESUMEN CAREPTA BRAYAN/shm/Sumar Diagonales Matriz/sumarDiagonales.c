#include<unistd.h>
#include<wait.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
//#define sizeElement 10000000

unsigned int sizeof_dm(int rows,int cols,size_t sizeElement)
{
    size_t size = rows*sizeof(void*);
    size +=(cols*rows*sizeElement);
    return size;
}
void create_index(void**m,int rows, int cols,size_t sizeElement){
    int i;
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for(i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}
int main(){
    __pid_t padre = getpid(), childs[3];
int rows=5,cols=5,r,c,i;
double **matrix, *suma = NULL ,shmId,sumaId;
size_t  sizeMatriz=sizeof_dm(rows,cols,sizeof(double));
sumaId = shmget(IPC_PRIVATE,sizeof(double),IPC_CREAT|0600);
shmId= shmget(IPC_PRIVATE,sizeMatriz,IPC_CREAT|0600);
matrix = shmat(shmId,NULL,0);
suma= shmat(sumaId,NULL,0);
create_index((void*)matrix,rows,cols,sizeof(double));

for(i=0;i<3;i++){
    childs[i]=fork();
    if(!childs[i]){
        break;
    }
}

if(getpid()==padre){
    int k =0;
        for(r=0;r<rows;r++){
            for(c=0;c<cols;c++){
                matrix[r][c] = k++;
            }
        }
        wait(NULL);
        printf("Soy el padre %d \n",getpid());
        for(r=0;r<rows;r++){
        for(c=0;c<cols;c++){
            printf("%.2f\t",matrix[r][c]);
        }
    printf("\n");
    }
    double sum_Total = 0.0;
    for( i = 0; i < 3; i++)
    {
        printf("La suma %d  es %.1f\n ",i+1, suma[i]);
        sum_Total += suma[i];
    }
    
    printf("la suma total es -> %.1f\t ",sum_Total);
    shmdt(matrix);
    shmdt(suma);
    shmctl(sumaId, IPC_RMID, 0);
    shmctl(shmId, IPC_RMID, 0);
}
else{
    sleep(2);

    if(i==0){
    double suma1 = 0.0;
    //printf("<-----soy el hijo 1 %d ---->\n",getpid());
    for(r=0;r<rows;r++){
        for(c=r+1;c<cols;c++){
            suma1+= matrix[r][c];
        }
    }
    //printf("La suma es %.1f\n Valor de i: %d\n", suma1, i);
    suma[i] = suma1;
    shmdt(matrix);
    shmdt(suma);

    }
    sleep(1);
    if (i == 1) {
        double suma2 = 0.0;
        //printf("<-----soy el hijo 2 %d ---->\n",getpid());
        for(r=0;r<rows;r++){
        for(c=0;c<cols;c++){
            if (r == c) {
                suma2 += matrix[r][c];
            }
            
        }
    printf("\n");
    }
    //printf("La suma es %.1f\n Valor de i: %d\n", suma2, i);
    suma[i] = suma2;
    shmdt(matrix);
    shmdt(suma);
    }
    
    if (i == 2) {
        sleep(1);
        double suma3 = 0.0;
    //printf("<-----soy el hijo 3 %d ---->\n",getpid());
    for(r=0;r<rows;r++){
        for(c=0;c<r;c++){
            suma3+= matrix[r][c];
        }
    }
    //printf("La suma es %.1f\n Valor de i: %d\n", suma3, i);
    suma[i] = suma3;
    shmdt(matrix);
    shmdt(suma);
    }
    
    
}
 // IMPRESION::  
    if(padre==getpid()){
        //sleep(2);
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(4);
    }
      //for(int j=0; j<n; j++) wait(NULL);
    // IMPRESION::

return 0;
}
