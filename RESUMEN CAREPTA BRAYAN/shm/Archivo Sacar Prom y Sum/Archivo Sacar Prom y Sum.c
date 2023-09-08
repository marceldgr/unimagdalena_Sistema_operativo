#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int k = 0, n, shmId, *vec, shmId2, i;
    float  *result;
    FILE *f = fopen("archivo.txt","r");
    if(!f) exit(-1);
    while(!feof(f)){
        fscanf(f,"%d",&n);
        k++;
    }
    fseek(f,SEEK_SET,SEEK_SET);
    shmId = shmget(IPC_PRIVATE,k*sizeof(int),0600);
    vec = shmat(shmId,NULL,0);
    shmId2 = shmget(IPC_PRIVATE, 3*sizeof(float),0600);
    result = shmat(shmId2,NULL,0);
    result[2] = -1;

    for(i = 1; i >= 0; i--) if(!fork()) break;
    
    if(i == -1){
        
        for(int a = 0; a < k; a++)
        {
            fscanf(f,"%d",&vec[a]);
        }
        result[2] += 1;
        fclose(f);
        
        for(int a = 0; a < 2; a++) wait(NULL);
        printf("promedio: %.0f\nsumatoria: %.0f\n",result[1], result[0]);
        shmdt(vec);
        shmdt(result);
        shmctl(shmId,IPC_RMID,NULL);
        shmctl(shmId2,IPC_RMID,NULL);        
    }else {
        if(i == 0){
            while(result[2] != 0) sleep(1);
            result[0] = 0;
            for(int a = 0; a <k; a++) result[0] += vec[a];
            result[2] += 1;
        }else{
            while(result[2] != 1) sleep(1);
            result[1] = (float)(result[0]/k);
        }
        shmdt(vec);
        shmdt(result);
    }

    return 0;
}
