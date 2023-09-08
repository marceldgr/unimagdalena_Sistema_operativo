/*
    JUAN DAVID RODRIGUEZ CERVANTES
    2013114113
    
    
 */
 #include<stdio.h>
 #include<unistd.h>
 #include<string.h>
 #include<stdlib.h>
 #include<wait.h>
 #include<sys/types.h>
 #include<sys/shm.h>
 #include<sys/stat.h>
 #include<signal.h>

 
 pid_t pid;
 int shm_idA,shm_idB,shm_idR;
 int m,n;
 int Rows=3;
 int Cols=3;
 double **matrixA,**matrixB,**matrixR;
 
 unsigned int sizeof_dm(int rows, int col, size_t sizeElement){
 size_t size = rows *sizeof(void *); 
 size += (col*rows*sizeElement); 
 return size;
 }

 void create_index(void **m, int rows, int cols, size_t sizeElement){
     int i;
     size_t sizerow = cols*sizeElement;
     m[0] = m+rows;
     
     for(i=1; i<cols; i++){
         m[i]= (m[i-1]+sizerow);
     }
 }

 int main(){

 size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(double));
 shm_idA = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
 shm_idB = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
 shm_idR = shmget(IPC_PRIVATE,sizeMatrix,IPC_CREAT|0600);
 matrixA=shmat(shm_idA, NULL, 0);
 matrixB=shmat(shm_idB, NULL, 0);
 matrixR=shmat(shm_idR, NULL, 0);// enlace
 
 create_index((void*)matrixA,Rows,Cols,sizeof(double));
 create_index((void*)matrixB,Rows,Cols,sizeof(double));
 create_index((void*)matrixR,Rows,Cols,sizeof(double));

     
 for(int m=0; m<Cols; m++){
     for(int n=0; n<Rows; n++){
         matrixA[m][n]=n/(double)(n + 1);
         matrixB[m][n]=(n + 1)/(double)(n + 2);
     }
 }
     
    pid = fork();
    if (pid != 0){  
        
        for(int m=0; m<Cols; m++){
            for(int n=0; n<Rows; n++){
                    matrixR[m][n]=matrixA[m][n]+matrixB[m][n];
            }
                   
        }
        shmdt(matrixA);
        shmdt(matrixB);
        shmdt(matrixR);
    }else{
        wait(NULL);
        for(int m=0; m<Cols; m++){
             for(int n=0; n<Rows; n++){
             printf("P A[%.6lf]",matrixA[m][n]);
             }
             printf("\n");
         }
         
         for(int m=0; m<Cols; m++){
             for(int n=0; n<Rows; n++){
             printf("P B[%.6lf]",matrixB[m][n]);
             }
             printf("\n");
         }
         
         for(int m=0; m<Cols; m++){
             for(int n=0; n<Rows; n++){
             printf("P R[%.6lf]",matrixR[m][n]);
             }
             printf("\n");
         }
                         
         shmdt(matrixA);
         shmdt(matrixB);
         shmdt(matrixR);
         shmctl(shm_idA,IPC_RMID,0);
         shmctl(shm_idB,IPC_RMID,0);
         shmctl(shm_idR,IPC_RMID,0);
         }
         return 0;
 }