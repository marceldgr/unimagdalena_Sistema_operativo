#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>

void mensaje(int codigo){
    switch (codigo) {
        case 0:
            printf("No hay problemas\n");
            break;
        case 1:
            printf("Un Buster exedio la temperatura! va a explotar!!!\n");
            break;
        case 2:
            printf("Abortando.. Los astronautas seran lanzados.\n");
            break;
        case 3:
            printf("Se ha llegado al espacio. Liberando capsula. \n");
            break;
        case 4:
            printf("Termine.\n");
            break;
        default :
            printf("Codigo no encontrado\n");
            break;
    }
}

int main(){
    int i, codigo=0, fd[4][2];
    // char buffer[50];

    for (int j=0; j<4; j++) if ( pipe(fd[j]) ) exit(-1);
    for (i=0; i<2; i++) if (!fork()) break;

    if (i == 2){
        char b[500];
        sprintf(b,"pstree -lp %d", getpid());
        system(b);
    } else sleep(3);


    if (i == 2){ //proceso padre
        for (int a=0; a<4; a++){
            if (a != 1 && a != 3) close(fd[a][0]);
            if (a != 0 && a != 2) close(fd[a][1]);
        }

        read(fd[1][0], &codigo, sizeof(int));
        printf("[%d] Padre: ", getpid());
        mensaje(codigo);
        if (codigo == 1){
            codigo = 2;
            write(fd[2][1], &codigo, sizeof(int));
        }else{
            codigo = 4;
            write(fd[0][1], &codigo, sizeof(int));
            codigo = 3;
            write(fd[2][1], &codigo, sizeof(int));
        }


        // for (int z=0; z<2; z++) wait(NULL);

        for (int a=0; a<4; a++){
            if (a != 0 && a != 2) close(fd[a][0]);
            if (a != 1 && a != 3) close(fd[a][1]);
        }
    }else{ //procesos hijos
        if (i == 0){
            for (int a=0; a<4; a++){
                if (a != i)  close(fd[a][0]);
                if (a != (i+1))  close(fd[a][1]);
            }

            int buster[4];
            int segundos = 5;
            int inicio = time(NULL);
            int final = inicio;

            srand(time(NULL));

            printf("Espere %d segundos...\n", segundos);
            printf("[%d] Hijo %d: Temperatura de los Busters \n", getpid(), i+1);

            do{
                final= time(NULL);
                for(int a = 0; a < 4; a++){
                    buster[a] = rand()%7;
                }

                for(int j = 0; j < 4; j++){
                    printf("\t[%d]=%d", j+1, buster[j]);
                    if(buster[j] > 5){
                        codigo = 1;
                        break;
                    }
                }
                printf("\n");
                if (codigo == 1) break;
                else sleep(1);
            } while( (final-inicio) < segundos);



            write(fd[1][1], &codigo, sizeof(int));
            read(fd[0][0], &codigo, sizeof(int));
            if (codigo != 1){
                printf("[%d] Hijo %d: ", getpid(), i+1);
                mensaje(codigo);
            }


            close(fd[i][0]);
            close(fd[i+1][0]);
        }
        if (i == 1){
            for (int a=0; a<4; a++){
                if (a != i+1) close(fd[a][0]);
                if (a != i+2) close(fd[a][1]);
            }

            read(fd[2][0], &codigo, sizeof(int));
            printf("[%d] Hijo %d: ", getpid(), i+1);
            mensaje(codigo);
            if (codigo != 2){
                printf("[%d] Hijo %d: ", getpid(), i+1);
                codigo = 4;
                mensaje(codigo);
            }

            close(fd[i+1][0]);
            close(fd[i+2][1]);
        }
    }

    return 0;
}
