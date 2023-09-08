/*
    ENUNCIADO:
        Utilizando [2] tuberias, crear el siguiente arbol de procesos (A,B) (B,C) en total seran [3] procesos.
        - El proceso (A) enviara un Array al proceso (B)
        - El proceso (B) Sacara el promedio.
        - El proceso (B) enviara el Array que recibio del proceso (A) al proceso (C)
        - El proceso (C) Sacara la moda del array "El valor que mas se repite" y la enviara al proceso (B)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // Libreria del wait

/*
fork() == 0 - Proceso hijo // fork() != 0: Proceso Padre
Cerra: [0] lectura - [1] Escritura

    getpid():  retorna Proceso Hijo ó id proceso actual.
    getppid(): retorna padre ó id proceso padre del proceso actual.

pwd
cd Escritorio/MaterialSO/PracticaSO
gcc ejercicio1_taller1.c
./a.out

Ver los procesos creados: ps -fe
*/

float promedio(int *v);
int moda(int *v);
void mostrar(int *v);

int main (){

    int i;
    int fd1[2],fd2[2],n;
    pipe(fd1);
    pipe(fd2);

    // Tuberias para (B) & (C)
    int fd3[2],fd4[2];
    pipe(fd3);
    pipe(fd4);

    for(i = 0; i < 2; i++){

        if(fork() != 0){ // Si es un proceso PADRE
            break;
        }
    }

    if(i == 0){
        // Tuberias De (A) & (B)
        close(fd1[0]); // Off lectura [0] - On  Escritura [1]
        close(fd2[1]); // On  lectura [0] - Off Escritura [1]

        // Tuberias De (B) & (C)
        close(fd3[1]); // On  lectura [0] - Off Escritura [1]
        close(fd4[0]); // Off lectura [0] - On  Escritura [1]


        int tam = 0;
        n = read(fd2[0], &tam, sizeof(tam));

        int vectt[tam];
        n = read(fd2[0], &vectt, sizeof(tam)*tam);
        printf("\n pid: %d - Proceso (B) leyendo Array -> Proceso (A)...\n TamArray: %d \n", getpid(), tam);
        //printf("\n2 - Fork(): Leido: tamVector = %d, BytesVect = %d \n", tam, sizeof(vectt));

        printf("\n pid: %d - Proceso (B) Enviando Array -> Proceso (C)...\n", getpid());
        write(fd4[1], &tam, sizeof(tam));
        write(fd4[1], &vectt, sizeof(vectt));
        wait(NULL);

        int moda;
        n = read(fd3[0], &moda, sizeof(moda));
        printf("\n\t pid: %d - Moda: [ %d ] \n",getpid(), moda);


        printf("\n pid: %d - Enviando promedio...\n", getpid());
        float prom = promedio(vectt);
        write(fd1[1], &prom, sizeof(prom));


    }else{

        if(i == 1){
            // Eliminar - Tuberias de (A) & (C)
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);
            close(fd2[1]);

            // Tuberias de (B) & (C)
            close(fd3[0]); // Off lectura [0] - On   Escritura [1]
            close(fd4[1]); // On  lectura [0] - Off  Escritura [1]



            int tam = 0;
            n = read(fd4[0], &tam, sizeof(tam));

            int vect[tam];
            n = read(fd4[0], &vect, sizeof(tam)*tam);
            //printf("\n3 - Fork(): Leido: tamVector = %d, BytesVect = %d \n", tam, sizeof(vect));
            printf("\n pid: %d - Proceso (C) leyendo Array -> Proceso (B)...\n TamArray: %d \n", getpid(), tam);

            printf("\n pid: %d - Enviando Moda...\n", getpid());
            int respModa = moda(vect);
            write(fd3[1], &respModa, sizeof(respModa));


        }else{
            // i == 2
            // Tuberia de comunicacion entre (A) & (B)
            close(fd1[1]); // On  lectura [0] - Off Escritura [1]
            close(fd2[0]); // Off lectura [0] - On  Escritura [1]

            // Cierro tuberia de (B) & (C)
            close(fd3[0]);
            close(fd3[1]);
            close(fd4[0]);
            close(fd4[1]);

            int sw = 0, tam = 0, tope = 1;
            int t, dato;

            FILE *file = fopen("datos.txt", "r");

            t = fscanf(file, "%d", &dato);
            if(t < 0){
                return 0;
            }

            int vector[dato];
            vector[0] = dato;

            do{
                t = fscanf(file, "%d", &dato);
                if(t < 0){
                    break;
                }else{
                    vector[tope] = dato;
                    tope++;
                }
                //printf("\n\t Imprime: [ %d ]", dato);

            }while(!feof(file));
            fclose(file);

            printf("\n pid: %d - Proceso (A) Enviando Array -> Proceso (B)...\n", getpid());
            /*printf("\n Mostrando.. . \n");
            mostrar((int*)vector);

            printf("\n Promedio: [ %.2f ]", promedio((int*)vector));
            printf("\n Moda: [ %d ]", moda((int*)vector));*/
            write(fd2[1], &vector[0], sizeof(vector[0]));
            //printf("1 - Fork(): pid: [%d] Escribio: tam= %d, bytes: %d\n", getpid(), vector[0], sizeof(vector[0]));

            write(fd2[1], &vector, sizeof(vector));
            //printf("1.1 - Fork(): pid: [%d] Escribio: tamVect= %d, bytes: %d\n", getpid(), vector[0], sizeof(vector));
            wait(NULL);

            float prmdo =0;
            n = read(fd1[0], &prmdo, sizeof(prmdo));
            printf("\n\t pid: %d Moda: [ %.2f ]", getpid(), prmdo);

            }
    }

//while(1);

printf("\n\n\n");
return 0;
}



void mostrar(int *v){

    printf("\n\t TMX del array: [ %d ] \n", v[0]);
    for(int i = 1; i <v[0]; i++){
        printf(" [ %d ]", v[i]);
    }
}

float promedio(int *v){

    int acu = 0;
    for(int i = 1; i < v[0]; i++){
        acu += v[i];
    }


return (float)acu/(v[0]-1);
}


int moda(int *v){

    int valRep = 0, cantRep = 0, cont;

    for(int i = 1; i < v[0]; i++){

        cont = 0;
        for(int j = 1; j < v[0]; j++ ){
            if(i != j && v[i] == v[j]){
                cont++;
            }
        }

        if(v[i] != valRep && cont > cantRep){
            valRep = v[i];
        }
    }

return valRep;
}
