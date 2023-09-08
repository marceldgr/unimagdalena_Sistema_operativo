#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>




int i,j,k,c1=0,c2=0,c3=0,c4=0;
pthread_t idhilo[4];

void * funcion_manejadore(void *);
void * funcion_manejadore1(void *);
void * funcion_manejadore2(void *);
void * funcion_manejadore3(void *);
	struct nodo
	{
		//int a,b;
	int matrizA [3][3];
	int matrizB [3][3];
	int matrizC [3][3];
	};
int main(){
pthread_t idhilo[4];

struct nodo *N;

	/*printf("Digite A\n");
	scanf("%d",&N->a);
	printf("Digite B\n");
	scanf("%d",&N->b);*/
srand(time(NULL));
N=(struct nodo*)malloc(sizeof(struct nodo));
for(i=0;i<3;i++){
	for(j=0;j<3;j++){
		N->matrizA[i][j]=rand()%10;		
		//N->a=i*2;
		//N->b=i*4;
	
	
	//pthread_create(&idhilo[i],NULL,funcion_manejadore,(void*)N);

	}}
for(i=0;i<3;i++){
	for(j=0;j<3;j++){
//		N=(struct nodo*)malloc(sizeof(struct nodo));
		N->matrizB[i][j]=rand()%10;		
		//N->a=i*2;
		//N->b=i*4;
	
	
	//pthread_create(&idhilo[i],NULL,funcion_manejadore,(void*)N);

	}}

/*
printf("\nMATRIZ RESULTANTE:\n");
        for(i=0;i<3;i++){
            printf("|\t");
            for(j=0;j<3;j++){
                printf("%d\t",N->matrizA[i][j]);
            }
            printf("|\n");
        }
printf("\nMATRIZ RESULTANTE:\n");
        for(i=0;i<3;i++){
            printf("|\t");
            for(j=0;j<3;j++){
                printf("%d\t",N->matrizB[i][j]);
            }
            printf("|\n");
        }*/
	//turno++;




//for(i=0;i<4;i++){
	pthread_create(&idhilo[1],NULL,funcion_manejadore,(void*)N);
	pthread_create(&idhilo[2],NULL,funcion_manejadore1,(void*)N);
	pthread_create(&idhilo[3],NULL,funcion_manejadore2,(void*)N);
	pthread_create(&idhilo[4],NULL,funcion_manejadore3,(void*)N);
	
//}
int sw=0;
	while(sw!=-1){
		if(c4==1){
			sw=-1;		
		}
	}
	
}
void* funcion_manejadore(void *param){

	
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(i==j){				
				((struct nodo *)param)->matrizC[i][j]=0;
				for(k=0;k<3;k++){
					((struct nodo *)param)->matrizC[i][j]=(((struct nodo *)param)->matrizC[i][j]+(((struct nodo *)param)->matrizA[i][k]*((struct nodo *)param)->matrizB[k][j]));
				}		
			}
	}
}		
	c1=1;
	}	
void* funcion_manejadore1(void *param){	
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(i>j){				
				((struct nodo *)param)->matrizC[i][j]=0;
				for(k=0;k<3;k++){
					((struct nodo *)param)->matrizC[i][j]=(((struct nodo *)param)->matrizC[i][j]+(((struct nodo *)param)->matrizA[i][k]*((struct nodo *)param)->matrizB[k][j]));
				}		
			}
	}
}
	c2=1;
	}
void* funcion_manejadore2(void *param){
	for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				   if(i<j){				
				((struct nodo *)param)->matrizC[i][j]=0;
				for(k=0;k<3;k++){
					((struct nodo *)param)->matrizC[i][j]=(((struct nodo *)param)->matrizC[i][j]+(((struct nodo *)param)->matrizA[i][k]*((struct nodo *)param)->matrizB[k][j]));
						}		
					  }
	  				}
				}
c3=1;
	
}


void* funcion_manejadore3(void *param){

int sw=0;

while(sw!=-1){	

	if(c1==1 && c2==1 && c3==1){

	
	pthread_join(idhilo[i],NULL);
	
		printf("\nMATRIZ RESULTANTE CCCC:\n");
        for(i=0;i<3;i++){
            printf("|\t");
            for(j=0;j<3;j++){
                printf("%d\t",((struct nodo *)param)->matrizC[i][j]);
            }
            printf("|\n");
        }
	}
            sw=-1;
	}
	c4=1;
}


/*if(turno==1){
		s=((struct nodo *)param)->a+((struct nodo *)param)->b;
printf("RE= %d \n",s);
	}else if(turno==2){

		r=((struct nodo *)param)->a-((struct nodo *)param)->b;
printf("RE= %d \n",r);	
}else{
		m=s*r;
printf("RE= %d \n",m);
		
	}turno++;

*/

