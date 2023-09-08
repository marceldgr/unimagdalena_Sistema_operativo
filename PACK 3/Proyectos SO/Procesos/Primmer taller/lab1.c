/*
    Creado por Daniel Vega - 2017214084 - 16/04/21
    notas:
        - El programa soporta trabajar con mas de 2 hijos. (N_CHILD)
        - El programa no hará la suma si la cantidad de numeros no es igual o mayor a los numeros de hijos definidos (ya que se espera seguramente que dada hijo trabaje en sumar así sea un elemento).
        - Dejé en el programa algunos comentarios que pueden aportar informacion importante a la hora de ejecutarlo así como tambien expliqué la logica que usé.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define N_CHILD 2
#define F_OUT "out.txt"
#define F_IN "input.txt"

struct  Child
{
    int start;
    int end;
}Childs[N_CHILD];

void ErrorReport(char*);
void ErrorReport_wpid(char* , int );
void AsingRangeChild(struct Child [],int,int);
int ReadFile(char* name, int ** vector);
int WorkChild(FILE * file_w, struct Child child, int vector[]);
int WorkFather(char*);

int main( void ) 
{
    remove(F_OUT);
    
    pid_t pidchild ,wpid;
    int  delta , indexAssign;
    int *data;

    FILE *file_w;
    file_w = fopen(F_OUT, "w");


    if (!file_w)
    {
      ErrorReport("Error al abrir el archivo de data.");
    }

    int amount_num = ReadFile(F_IN, &data); 

    if(amount_num < N_CHILD)
    {
        ErrorReport("Error, el archivo tiene menos valores que la cantidad de hijos.");
    }

    delta = amount_num / N_CHILD;

    AsingRangeChild(Childs,delta,amount_num);

    indexAssign = 0;

    //Crear los hijos dependiendo del valor en N_child. en este caso para el laboratorio son 2.

    for(int i = 0; i < N_CHILD ; i++)
    {

        pidchild = fork();

        if(!pidchild)
        {
            break;
        }
        else{
            indexAssign++; //como las variables se comparten entonces la variable indexAssign me ayudará a saber que accion debe realizar el hijo.
        }

    }
	 

    switch (pidchild)  
	{ 
        case -1:   
            ErrorReport("Error Fork.");
            exit(EXIT_FAILURE);
        case 0: // Bloque proceso hijo  
                //printf( "Proceso hijo\n" ); 
                printf("Soy el hijo con id:%d y me toca ir de %d hasta %d\n",getpid(),Childs[indexAssign].start,Childs[indexAssign].end);
		        WorkChild(file_w,Childs[indexAssign],data);
                break;
        default: /// Bloque proceso padre 
                //printf( "Proceso padre\n" ); 
                for(int i = 0; i < N_CHILD;i++){
                    wpid = wait(NULL); //La idea es que no pueda hacer nada hasta que no endalice el trabajo de los hijos
                    //printf("endalizó el hijo: %d\n", wpid);
                }
                fclose(file_w);
                WorkFather(F_OUT);

                //printf("endalizó el padre: %d\n", getpid());
 	}
	return 0;
}

int WorkChild(FILE * file_w, struct Child  child, int vector[])
{
    int add = 0;
    
    for(int i = child.start; i <= child.end; i++){
            add += vector[i];
        }

    fprintf(file_w, "%d %d\n",add,getpid());
    return EXIT_SUCCESS;
}

int WorkFather(char* name)
{
    FILE *file;
    file = fopen(name, "r");

    if (!file)
    {
          ErrorReport_wpid("Error al abrir el archivo de salida desde el padre. pid:",getpid());
    }

    int total = 0,addChild = 0,pid;

    for(int i = 0; i < N_CHILD;i++)
    {
        fscanf(file, "%d %d", &addChild, &pid);
        total += addChild;
        printf("El hijo %d sumó: %d\n",pid,addChild );
    }

    printf("Suma total de los elementos del vector: %d\n",total);
    fclose(file);

    
}

void AsingRangeChild(struct Child vector[], int delta,int cantidad)
{


   for(int i = 0; i < N_CHILD; i++)
   {

    if( i == 0)
    {
        vector[i].start =  0;
        vector[i].end = delta-1;
    }

    else
    {
        vector[i].start = vector[i-1].end + 1;

        if(i == N_CHILD-1)
        {
            vector[i].end = cantidad-1;
        }
        else
        {
            vector[i].end = vector[i].start + (delta-1);
        }
        
    }

   }

}

int ReadFile(char* name, int ** vector)
{
   FILE *file;
   file = fopen(name, "r");

   if (!file)
   {
          ErrorReport("Error al abrir el archivo de data");
   }

   int rows, value , c = 0 ;

    fscanf(file, "%d", &rows);

    *vector = (int *)calloc(rows, sizeof(int));
    
    if(!*vector){
        ErrorReport("Error con la asignacion de memoria para el vector de archivo.");
    }
    for(c = 0; c < rows; c++)
    {
        fscanf(file, "%d", &value);
        (*vector)[c] = value;
        //printf("%d\n",value );   
    }


   fclose(file);
   
   return c;
}



void ErrorReport(char* msg){
    perror(msg);
    exit(EXIT_FAILURE);

}

void ErrorReport_wpid(char* msg, int wpid){
    fprintf(stderr, "%s %d", msg,wpid);
    exit(EXIT_FAILURE);

}


