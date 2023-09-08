#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h>
#include <string.h> 
#include <sys/types.h> 
#define BUFFER 1024

int **CrearTuberias(int);
//Deben estar en orden.
void Cerrar_Descriptores_RW(int **, int , int * ,int);
void Cerrar_Descriptores_RW_int(int **, int , int ,int);

//void bubble_sort(int list[], int n)

int main() 
{
    int **Pipes; //[][0] read , [][1] write
    int Cantidad = 0;
    int i = 0;

    printf("Ingrese la cantidad de Hijos: ");
    scanf("%d",&Cantidad);

    if (Cantidad < 1)
    {
        printf("Disculpa, el valor de los hijos debia ser mayor que 0");
        exit(EXIT_FAILURE);
    }
    
    Pipes = CrearTuberias(Cantidad);

    char DataIn[BUFFER];

    for (; i < Cantidad; i += 1) //ejecuta a ver ? me dice violacion de segmento pero no sé que está violando. es decir , eso no deveria violarlo.
    {
        if(!fork()) break; //no vi que escribiste xd  andaba mirando otro codigo, entra al chat de voz
        
    }

     if(i == Cantidad) //padre
    { 
       char DataOut[BUFFER];
       printf("Mensaje: ");

       scanf("%s",DataOut);
    
       int lectura = Cantidad*2-1;
       int escritura = 0;

       Cerrar_Descriptores_RW_int(Pipes,Cantidad*2,lectura,0); //lectura del ultimo hijo
       Cerrar_Descriptores_RW_int(Pipes,Cantidad*2,escritura,1); //escritura al primer hijo.

       printf("El padre envió el mensaje {%s}\n",DataOut);
       write(Pipes[escritura][1],&DataOut,BUFFER);
       close(Pipes[escritura][1]);

       read(Pipes[lectura][0],&DataIn,BUFFER);
       close(Pipes[lectura][0]);
       printf("El padre recibó el mensaje {%s}\n",DataIn);

       for (size_t i = 0; i < Cantidad; i++)
       {
           wait(NULL);
       }

    }
    else if(i == Cantidad-1) //Caso especial ultimo hijo
    {
        usleep(400);
        int lectura = i;
        int escritura = i+1;
        Cerrar_Descriptores_RW_int(Pipes,Cantidad*2, escritura,1);//escritura
        Cerrar_Descriptores_RW_int(Pipes,Cantidad*2, lectura,0);//lecutra segunda mitad.

        read(Pipes[lectura][0],&DataIn,BUFFER);
        printf("El hijo raro recibó el mensaje {%s}\n",DataIn);
        
        write(Pipes[escritura][1],&DataIn,BUFFER);
        close(Pipes[escritura][1]); close(Pipes[lectura][0]);

    }
    else
    {
        usleep(400);

        int escritura[] = {i+1,Cantidad*2-1-i};
        int lectura[] = {i,(Cantidad*2-2-i)};

        Cerrar_Descriptores_RW(Pipes,Cantidad*2, escritura,1);//escritura
        Cerrar_Descriptores_RW(Pipes,Cantidad*2, lectura,0);//lectura segunda mitad.

        //La primera mitad
        read(Pipes[lectura[0]][0],&DataIn,BUFFER);
        printf("El hijo {%d} recibó el mensaje {%s}\n",i,DataIn);
        write(Pipes[escritura[0]][1],&DataIn,BUFFER);
        close(Pipes[escritura[0]][1]); close(Pipes[lectura[0]][0]);
        //La segunda mitad.
        read(Pipes[lectura[1]][0],&DataIn,BUFFER);
        printf("El hijo {%d} recibó el mensaje {%s}\n",i,DataIn);
        write(Pipes[escritura[1]][1],&DataIn,BUFFER);
        close(Pipes[escritura[1]][1]); close(Pipes[lectura[1]][0]);

    }
        
        //read(Pipes[0][0],&Data,BUFFER);
        //printf("Mensaje recibido del padre: {%s}",Data);
    

    return EXIT_SUCCESS;
}

int **CrearTuberias(int Cantidad){
    int **Pipes = (int**)calloc(Cantidad*2,sizeof(int*));

    for(int i = 0; i  < Cantidad*2; i++)
    {
        Pipes[i] = (int*)calloc(2,sizeof(int));
    }

    for(int i = 0; i  < Cantidad*2; i++)
    {
        pipe(Pipes[i]);
    }



    return Pipes;  
}

//Cierra los descriptores de las tuberias menos los indices que llegan por el vector "IndexAbiertos"
void Cerrar_Descriptores_RW(int **Pipes, int size, int *IndexAbiertos,int escritura_lectura)
{
    //bubble_sort(IndexAbiertos,sizeof(IndexAbiertos)); //Es innecesario si no entran ordenados.
    int contador = 0;
    int sizeIndex = sizeof(IndexAbiertos)/sizeof(int);

    for (int i = 0; i < size; i++)
    {
        
        if(IndexAbiertos[contador] != i)
        {
            close(Pipes[i][escritura_lectura]);
        }
        else //Si el index que se envia concuerda con la variable i entonces pasa al siguiente valor que contiene el indexAbierto.
        {
            if(contador < sizeIndex )
            {
                contador++;
            }
        }
    }
    
}

void Cerrar_Descriptores_RW_int(int **Pipes, int size, int IndexAbiertos,int escritura_lectura)
{

    for (int i = 0; i < size; i++)
    {
        
        if(IndexAbiertos != i)
        {
            close(Pipes[i][escritura_lectura]);
        }

    }
    
}
