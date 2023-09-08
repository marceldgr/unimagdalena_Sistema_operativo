#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIMITE 500
#include <time.h>

// Ejemplo curso C aprenderaprogramar.com
int main() {
    int i,j,n=4;
    int letras[4][4]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    printf("---Original---\n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d",letras[i][j]);
        }
        printf("\n");
    }
    printf("-------diagonal-------\n");
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(i==j){
               printf("%d",letras[i][j]);
            }
            else{
                printf("%c",' ');
            }       }
        printf("\n");
    }

    printf("-------1er-------\n");
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            //printf("%d",letras[i][j]);
            letras[i][j]=1;
        }
    }
     for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d",letras[i][j]);
        }
        printf("\n");
    }
    printf("-------2er-------\n");
    for(i=1;i<n;i++){
        for(j=0;j<i;j++){
            //printf("%d",letras[i][j]);
            letras[i][j]=2;
        }
    }
     for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d",letras[i][j]);
        }
        printf("\n");
    }    
    return 0;
}