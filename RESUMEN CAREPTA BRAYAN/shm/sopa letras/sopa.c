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
    pid_t padre = getpid(), childs[3];
    int n;
    //leer archivo
    FILE * f = fopen("sopa.txt", "r");
    if(!f) exit(-1);
    fscanf(f, "%d", &n);
    int r,i,c;
    char **M = NULL;
    char linea[n];
    int shmId;
    size_t  sizeMatriz=sizeof_dm(n,n,sizeof(char));

    shmId= shmget(IPC_PRIVATE,sizeMatriz,IPC_CREAT|0600);
    M = shmat(shmId,NULL,0);
    create_index((void*)M,n,n,sizeof(char));

    fseek(f,2,0);
    for( i = 0; i < n; i++)
    {
        fgets(linea, n+1, f);
        fgetc(f);//salto de linea
        for( int j = 0; j < n; j++)
        {
            M[i][j] = linea[j];
        }        
    }
    for(r=0;r<n;r++)
    {
        for(c=0;c<n;c++)
        {
            printf("%c ",M[r][c]);
        }
        printf("\n");
    }
    if(padre==getpid()){
        
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else{
        sleep(1);
        }

return 0;
}