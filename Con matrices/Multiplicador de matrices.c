#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n;/*dimension de la matriz... n por n*/
int **M1,**M2,**Mr;/*Matrices dinmámicas.... factores y resultado*/
pthread_t hiloid[3];/*Vector que almacena los id de los 3 hilos: 1 y 2 son calculadores el 3 es el impresor*/

/*-------FUNCION PARA LOS 3 HILOS-------*/
/*en esta funcion el agrumento identifica que clase de hilo es.... para luego definir las acciones que corresponde,
para los caladores se le paso 1 para el calculadore de posiciones pares y 2 para el de las impares,
el hilo impresor se e pasó argumento con valor 3*/

void * run(void *arg){
	int *a = (int*)arg;/*se recive el argumento*/
	int b = 0;/*Bandera que.... jajajaja no se como explicarlo*/
	/*Mas o menos... esta variable controla que los saltos en las columnas sean pares o impares,
	si esta variable es 1 los saltos serán impares y 0 para los saltos pares*/

	int i,j,k,r;/*indices para los for*/
	/*el i, j y k se usan para la multiplicacion de matrices*/
	/*La variable k controla los saltos de fila en la matriz resultado, y los saltos de fila de la matriz 1*/
	/*La varuiable i controla los saltos de columna en la 
	matriz de resultado y en la segunda matriz... siempre se haran de 2 en 2*/

	/*La vatiable j controla los saltos de fila en la matriz 2 y los saltos de columna en la matriz 1*/

	/*el r se usa en el impresor para esperar a que los calculadores terminen sus funciones*/

	if(*a==1||*a==2){/*si eres un calculador... haga*/
		if(*a==1){/*si eres el calculador de pares....haga*/
			b=0;/*los saltos de columna iniciarán en pares...*/
		}else if(*a==2){/*si eres el calculador de impares.... */
			b=1;/*los saltos de columna comensarán en impares*/
		}

		/*Independientemente de que calculador eres multiplicarás....*/
		
		/*----- codigo multiplicador-------*/
		for(k=0;k<n;k++){		
			for(i=0;i<n;i+=2){/*los saltos de i seran de 2 en 2*/
				for(j=0;j<n;j++){
					Mr[k][i+b]+=M1[k][j]*M2[j][i+b];
					/*Notar que si b es 0 los saltos son pares y si es uno serán impares*/
				}
			}

			/*Si ya terminaste de calcular las celdas que te corresponden en una fila de la matriz resultado*/
			if(b==1){/* y además tus saltos de columna son impares*/
				b=0;/*pasalos a pares*/
			}else{/*si no... es decir que tus saltos son pares*/
				b=1;/*pasalos a impares*/
			}
		}
		/*---------Codigo multimplicador de las matrices----------*/
	
	}else if(*a==3){/*si eres impresor*/
		for(r=0;r<2;r++){/*espera a los otros dos hilos*/
			pthread_join(hiloid[r],NULL);
		}
		/*Imprime el resultado*/
		printf("\nMATRIZ RESULTANTE:\n");
		for(i=0;i<n;i++){
			printf("|\t");
			for(j=0;j<n;j++){
				printf("%d\t",Mr[i][j]);
			}
			printf("|\n");
		}
	}
	pthread_exit(NULL);/*Independiemte mente de quien seas... sal de la función*/
}

//MAIN PRINCIPAL----------------------------------
int main(){
	int i,j,*a;	

	//PIDE EL TAMAÑO DE LA MATRIZ----------------------------------
	printf("Escriba el tamaño de la matriz:\n");
	scanf("%d",&n);

	//DECLARA MATRICES DINAMICAS----------------------------------
	M1=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		M1[i]=(int*)malloc(n*sizeof(int));
	}

	M2=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		M2[i]=(int*)malloc(n*sizeof(int));
	}

	Mr=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		Mr[i]=(int*)malloc(n*sizeof(int));
	}

	//ASIGNA VALORES A MATRICES 1 Y 2 POR TECLADO----------------------------------
	printf("ingrese los datos para la matriz 1:\n");
	for(i=0;i<n;i++)
		for(j=0;j<n;j++){
			printf("M1[%d][%d]: ",i,j);
			scanf("%d",&M1[i][j]);
		}
	
	printf("ingrese los datos para la matriz 2:\n");
	for(i=0;i<n;i++)
		for(j=0;j<n;j++){
			printf("M2[%d][%d]: ",i,j);
			scanf("%d",&M2[i][j]);
		}


	//CREA HILOS----------------------------------	
	for(i=0;i<3;i++){
		a=(int*)malloc(sizeof(int));
		*a=i+1;
		
		pthread_create(&hiloid[i],NULL,run,(void*)a);
	}
	/*El hilo principal espera a los demás hilos*/
	for(i=0;i<3;i++){		
		pthread_join(hiloid[j],NULL);
	}
	return 0;	
}
