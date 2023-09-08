#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <ctype.h>


int main(){
    system("clear");
    int i, fd1[2], fd2[2], tam;
    char lectura;
    pid_t root = getpid();

    if (pipe(fd1)) exit(-1);
    if (pipe(fd2)) exit(-1);

    for (i=0; i<2; i++) if (!fork()) break;

    if(root == getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else sleep(3);

    if (i == 2){  // padre
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);

        int cont_saltos = 0;
        read(fd1[0], &tam, sizeof(int));

        for (int a=0; a<tam; a++){
            read(fd1[0], &lectura, sizeof(char));
            if (lectura == '\n') cont_saltos++;
        }
        cont_saltos--; //para quitar el salto al final del archivo
        printf("Padre  [%d]: El numero de saltos de linea es %d\n", getpid(), cont_saltos);

        close(fd1[0]);
    }else{
        if (i == 1){ //2do hijo
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[0]);

            FILE *f = fopen("taller_practica.txt", "r");
            if (!f) exit(-1);
            fseek(f, 0, 2);
            tam = ftell(f);
            fseek(f, 0, 0);

            write(fd2[1], &tam, sizeof(int));
            for (int a=0; a<tam; a++){
                lectura = fgetc(f);
                write(fd2[1], &lectura, sizeof(char));
            }
            fclose(f);
            printf("Hijo %d [%d]: Se ha enviado el archivo pór tuberia.\n", i+1, getpid());

            close(fd2[1]);

        }else{  // 1er hijo
            close(fd1[0]);
            close(fd2[1]);

            int cont_vocales = 0;
            read(fd2[0], &tam, sizeof(int));
            char *buffer;
            buffer = (char*)calloc(tam, sizeof(char));

            for (int a=0; a<tam; a++){
                read(fd2[0], &lectura, sizeof(char));
                buffer[a] = lectura;
                if ( lectura == 'a' || lectura == 'A' ||
                     lectura == 'e' || lectura == 'E' ||
                     lectura == 'i' || lectura == 'I' ||
                     lectura == 'o' || lectura == 'O' ||
                     lectura == 'a' || lectura == 'U' )  cont_vocales++;
            }
            printf("Hijo %d [%d]: El numero de vocales es %d\n", i+1, getpid(), cont_vocales);
            write(fd1[1], &tam, sizeof(int));
            for (int a=0; a<tam; a++) write(fd1[1], &buffer[a], sizeof(char));

            close(fd1[1]);
            close(fd2[0]);
        }
    }


    return 0;
}
