#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

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

int main()
{
    int n ;
    printf("digite n:\n");
    scanf("%d",&n);
    pid_t padre = getpid();
    int **a=NULL,**b=NULL,**result=NULL,shmid1,shmid2,shmid3,i;
    size_t sizeMatrix = sizeof_dm(n,n,sizeof(int));
    
    shmid1 = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    shmid2 = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    shmid3 = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT|0600);
    a =  shmat(shmid1,NULL,0);
    b =  shmat(shmid2,NULL,0);
    result =  shmat(shmid3,NULL,0);
    create_index((void*)a, n, n, sizeof(int));
    create_index((void*)b, n, n, sizeof(int));
    create_index((void*)result, n, n, sizeof(int));
    
    for( i = 0; i < 3; i++) if(!(fork()))break;
    if(padre == getpid()){
        int cont = 0;
        for(int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                a[r][c] = cont;
                b[r][c] = cont;
                cont++;
            }
        }
        printf("las matrices A y B son:\n");
        printf("A\n");
        for(int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                printf("[%d]",a[r][c]);
            }
            printf("\n");
        }
        printf("B\n");
        for(int r = 0; r < n; r++){
            for(int c = 0; c < n; c++){
                printf("[%d]",b[r][c]);
            }
            printf("\n");
        }
        printf("\n");
        sleep(4);
        printf("soy el padre y la matriz resultante es\n"); 
        for(int r = 0; r < n; r++)
        {
            for(int a = 0; a < n; a++){
            printf("[%d]",result[r][a]);
            }
        printf("\n");
        }
        printf("\n");
        shmdt(a);
        shmdt(b);
        shmdt(result); 
        shmctl(shmid1,IPC_RMID,0);
        shmctl(shmid2,IPC_RMID,0);
        shmctl(shmid3,IPC_RMID,0);
    }else{
        //sleep(1);
        if(i == 0){
            sleep(1);
            for(int r=1;r<n;r++){
                for(int j=0;j<r;j++){
                    result[r][j]=0;
                    for(int k=0;k<n;k++){
                        result[r][j]=(result[r][j]+(a[r][k]*b[k][j]));
                    }
                }
            }
            printf("soy el hijo%d\n",i+1); 
            for(int r = 0; r < n; r++)
            {
                for(int c = 0; c < n; c++){
                printf("[%d]",result[r][c]);
                }
            printf("\n");
            }
            printf("\n");
        }if(i == 1){
            sleep(2);
            for(int r=0;r<n;r++){
                for(int c=r+1;c<n;c++){
                    result[r][c]=0;
                    for(int k=0;k<n;k++){
                        result[r][c]=(result[r][c]+(a[r][k]*b[k][c]));
                        }
                    }
            }
            printf("soy el hijo%d\n",i+1); 
            for(int r = 0; r < n; r++)
            {
                for(int c = 0; c < n; c++)
                {
                printf("[%d]",result[r][c]);
                }
            printf("\n");
            }
            printf("\n");
            
        }
        if (i == 2) {
            sleep(3);
            for(int r=0;r<n;r++)
            {
                for(int c=0;c<n;c++)
                {                  
                    if(r==c)
                    {
                        result[r][c]=0;
                        for(int k=0;k<n;k++)
                        {
                            result[r][c]=(result[r][c]+(a[r][k]*b[k][c]));
                        }
                    }
                }
            }
            printf("soy el hijo%d\n",i+1); 
            for(int r = 0; r < n; r++)
            {
                for(int c = 0; c < n; c++)
                {
                printf("[%d]",result[r][c]);
                }
             printf("\n");
            }            

        }
        shmdt(a);
        shmdt(b);
        shmdt(result);

    }
    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(1);  
    }
    

    return 0;
}
