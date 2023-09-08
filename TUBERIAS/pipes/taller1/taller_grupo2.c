#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int main(int argc, char *argv[]){
    system("clear");
    pid_t childs[2], root = getpid();
    int i, fd1[2], fd2[2];

    if(pipe(fd1)) exit(-1);
    if(pipe(fd2)) exit(-1);

    for (i=0; i<2; i++) if(!(childs[i] = fork())) break;

    if (getpid() != root){
        if(i == 0 && childs[0] == 0){
            close(fd1[0]);
            close(fd2[0]);
            int fsize;

            FILE *file1 = fopen("file1.txt", "r");
            if (!file1){
                printf("Erro al abrir el archivo file1\n");
                exit(-1);
            }
            fseek(file1, 0, SEEK_END);
            fsize = ftell(file1);
            fseek(file1, 0, 0);

            int lsize;
            char *buffer;
            buffer = malloc((fsize)*sizeof(char));

            while(fgets(buffer, fsize, file1)){
                lsize = strlen(buffer);
                buffer[lsize-1] = '\0'; //cambio el salto de linea por el final de linea
                write(fd1[1], &lsize, sizeof(int));
                write(fd1[1], buffer, lsize);
            }
            fclose(file1);

            strcpy(buffer, "<<end1>>");
            lsize = strlen(buffer);
            write(fd1[1], &lsize, sizeof(int));
            write(fd1[1], buffer, lsize);


            FILE *file2 = fopen("file2.txt", "r");
            if (!file2){
                printf("Erro al abrir el archivo file2\n");
                exit(-1);
            }
            fseek(file2, 0, SEEK_END);
            fsize = ftell(file2);
            fseek(file2, 0, 0);

            buffer = malloc((fsize)*sizeof(char));

            while(fgets(buffer, fsize, file2)){
                lsize = strlen(buffer);
                buffer[lsize-1] = '\0'; //cambio el salto de linea por el final de linea
                write(fd2[1], &lsize, sizeof(int));
                write(fd2[1], buffer, lsize);
            }
            fclose(file2);

            strcpy(buffer, "<<end2>>");
            lsize = strlen(buffer);

            write(fd2[1], &lsize, sizeof(int));
            write(fd2[1], buffer, strlen(buffer));

            close(fd1[1]);
            close(fd2[1]);
        }
        if(i == 1 && childs[1] == 0){
            close(fd1[1]);
            close(fd2[1]);

            int lsize, sw = 0, end1=0, end2=0;
            char *buffer1, *buffer2;

            while(1){
                if (!end1){
                    read(fd1[0], &lsize, sizeof(int));
                    buffer1 = malloc((lsize)*sizeof(char));
                    read(fd1[0], buffer1, lsize);

                    if (!strcmp(buffer1, "<<end1>>")){
                        end1 = 1;
                        if (sw == 1) break;
                        else sw++;
                    }else{
                        for (int a=0; a<lsize; a++) printf("%c", buffer1[a]);
                        printf("\n");
                    }
                }

                if (!end2){
                    read(fd2[0], &lsize, sizeof(int));
                    buffer2 = malloc((lsize)*sizeof(char));
                    read(fd2[0], buffer2, lsize);

                    if (!strcmp(buffer2, "<<end2>>")){
                        end2 = 1;
                        if (sw == 1) break;
                        else sw++;
                    }else{
                        for (int a=0; a<lsize; a++) printf("%c", buffer2[a]);
                        printf("\n");
                    }
                }
            }

            close(fd1[0]);
            close(fd2[0]);
        }
    }else{
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
    }
    return 0;
}
