#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>


int num_hilos=0,filas,columnas,matriz[100][100],matriz2[100][100];
int tiempo;
void* funcion_hilo_filas(void*);
void* fun_m(void* arg);
int getValor(int x,int y);

int getPos(){
	for(int i=1;i<filas-1;i=i+1){
		for(int j=1;j<columnas-1;j=j+1){
			if(matriz2[i][j]==0){
				matriz2[i][j]=getValor(i,j);
				return 1;
			}
		}
	}
	return 0;
}

void inicializarMatrices(){
	for(int i=0;i<filas;i++){
		for(int j=0;j<columnas;j++){
			matriz[i][j]=0;
			matriz2[i][j]=0;
		}
	
	}
}




void ipm2(){
	for(int i=0;i<filas;i++){
		for(int j=0;j<columnas;j++){
			printf("[%d]",matriz2[i][j]);
		}
		printf("\n");
	}
}
void ipm1(){
	for(int i=0;i<filas;i++){
		for(int j=0;j<columnas;j++){
			printf("[%d]",matriz[i][j]);
		}
		printf("\n");
	}
}
int cortarCadena(char cad[100],char caracter, int veces){
	veces=veces-1;
	for(int i=0;i<100;i++){
		if(cad[i]==caracter){
			if(veces==0){
				return i;
			}
			veces=veces-1;
		}
	}
	return 0;
}

int leerDatos(char ruta[20]){
	FILE *f=fopen(ruta,"r");
	if(f!=NULL){
		char data[100];
		fgets(data,100, f);
		int pos =cortarCadena(data,'\n',1);
		if(pos==0){
			printf("error leyendo num_hilos");
			return -1;
		}
		data[pos]='\0';
		tiempo=atoi(data);

		
		fgets(data,100, f);
		char separador[]=" ";
		char *tokend=strtok(data,separador );
		filas = atoi(tokend);
		tokend=strtok(NULL,separador);
		columnas=atoi(tokend);

		for(int i=0;i<filas;i++){
			fgets(data,100,f);
			int j=0;
			tokend=strtok(data,separador);
			while(tokend!=NULL){
				matriz[i][j]=atoi(tokend);
				j=j+1;
				tokend=strtok(NULL,separador);
			}
		}
		ipm1();
		return 1;
		
	}else{
		printf("error de lectura");
	}
	return 0;
}
int getValor(int x,int y){
	if((x>0 && x<filas-1) && (y>0 && y<columnas-1)){
		//x para columnas y y para filas
		return (matriz[x-1][y]+matriz[x+1][y]+matriz[x][y-1]+matriz[x][y+1])/4;
	}else{
		return 0.0;
	}
}
pthread_barrier_t mybarrier;
int main(){
	inicializarMatrices();
	char ruta[20]="datos.in";
	if(leerDatos(ruta)==1){
		
		printf("escriba el numero de hilos :");
		scanf("%d",&num_hilos);

		pthread_t tid[num_hilos];
		int var=(filas*filas)/num_hilos;

		for(int i=0; i<num_hilos; i++){
			
			pthread_create(&tid[i], NULL, funcion_hilo_filas, (void*)&var);
		}
		

		for(int i=0; i<num_hilos; i++){
			pthread_join(tid[i], NULL);
		}
		pthread_t idM=pthread_self();
		pthread_create(&idM, NULL, fun_m, NULL);
		pthread_join(idM, NULL);
		// printf("sisas");

for(int i=1;i<filas-1;i++){
	for(int j=1;j<columnas-1;j++){
	matriz[i][j] = matriz2 [i][j];
}
}

		// ipm2();
		ipm1();
		
		
	}else{
		return -1;
	}

	printf("terminando padre");
	exit(0);
}
void* fun_m(void* arg){
	int i=0;
	while(i<3){
		sleep(tiempo);
		ipm2();
		i=i+1;
	}
	//printf("sisa");
}
void* funcion_hilo_filas(void* arg){
	int *data= (int *)arg;
	int t;
	for(int i=0;i<*data;i++){
		t=getPos();
	}
//	printf("\n %d index=%lu",*data,pthread_self());
	pthread_exit(0);
}
