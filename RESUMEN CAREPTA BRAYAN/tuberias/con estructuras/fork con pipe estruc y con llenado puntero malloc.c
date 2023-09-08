#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct msj{
	int a;
	float b;
}msj;

msj* Crearmsj(int a, float b){
	msj* MSJ=(msj*)malloc(sizeof(msj));
	if(MSJ==NULL) printf("lleno");
	MSJ->a=a;
	MSJ->b=b;
	return MSJ;
}

msj* CrearmsjLimpio(){
	msj* MSJ=(msj*)malloc(sizeof(msj));
	if(MSJ==NULL) printf("lleno");
	MSJ->a=NULL;
	MSJ->b=NULL;
	return MSJ;
}

void MostrarMsj(msj* MSJ){
	printf("soy el entero :%d \n soy el flotante:%f",MSJ->a,MSJ->b);
}

int main(){
    pid_t idProceso;
	int tuberia[2],estadoHijo;
//	msj* MSJX=CrearmsjLimpio();
	msj* MSJX1;
	
	if (pipe (tuberia) == -1)
	{
		perror ("No se puede crear Tuberia");
		exit (-1);
	}
	
	
	idProceso=fork();
	
		if (idProceso == -1){
		perror ("No se puede crear proceso");
		exit (-1);
	   }
	
	if(idProceso==0){
	    close(tuberia[1]);
		//struct msj var;
		
		read(tuberia[0],(void*)MSJX1,sizeof(msj));
		MostrarMsj(MSJX);
		//printf("Entero:%d...Flotante:%f",MSJ->a,MSJ->b);
		exit (0);
		//printf("Leido: int(%d), float(%f)\n", var.a, var.b);}
   }else{
		//struct msj b;
		close(tuberia[0]);
		//MSJX=Crearmsj(2,2.3);
		MSJX1=Crearmsj(2,2.3);
		//b.a =10;
		//b.b=2.3;
		write(tuberia[1],(void*)MSJX1, sizeof(msj));
		wait(&estadoHijo);
		exit (0);
	}
	return 0;
}
