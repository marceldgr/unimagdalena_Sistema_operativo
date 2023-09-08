#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>

int main()
{
    pid_t childs[3], padre = getpid();
    int i, j=-1, n, npipes = 6;
    int fd[npipes][2]; //filas (n tuberias), columnas


    for(int k=0; k<npipes; k++) if(pipe(fd[k])) exit(-1);

    for (i=0; i<3; i++){
        if (!(childs[i] = fork())){
            if (i == 2){
                for(j=0; j<2; j++)
                    if ((childs[j] = fork())) break;
            }
            break;
        }
    }

    if (getpid() != padre){

        if (j > 0){
            for (int k=0; k<npipes; k++){
                if ((j+2) != k) close(fd[k][0]);
                if ((j+3) != k) close(fd[k][1]);
            }
        }else{
            for (int k=0; k<npipes; k++){
                if (i != k) close(fd[k][0]);
                if ((i+1) != k) close(fd[k][1]);
            }
        }

        int TAM, c=0;

        if (j > 0){
            read(fd[j+2][0], &TAM, sizeof(int));
            char info[TAM];

            for (int k=0; k<TAM; k++){
                read(fd[j+2][0], &info[k], sizeof(char));
                if (j == 1) if (info[k] == '.') c++;
                if (j == 2) if (info[k] == ',') c++;
            }
            if (j == 1)
                printf("proceso %d: Hay %d punto(s)\n", getpid(), c);
            if (j == 2)
                printf("proceso %d: Hay %d coma(s)\n", getpid(), c);

            write(fd[j+3][1], &TAM, sizeof(int));
            for (int k=0; k<TAM; k++){
                write(fd[j+3][1], &info[k], sizeof(char));
            }
            close(fd[j+2][0]);
            close(fd[j+3][1]);
        }else{
            read(fd[i][0], &TAM, sizeof(int));
            char info[TAM];

            for (int k=0; k<TAM; k++){
                read(fd[i][0], &info[k], sizeof(char));
                if (i == 0) if (info[k] >= 'A' && info[k] <= 'Z') c++;
                if (i == 1) if (info[k] >= 'a' && info[k] <= 'z') c++;
                if (i == 2) if (info[k] == ' ') c++;
            }

            if (i == 0)
                printf("proceso %d: Hay %d mayuscula(s)\n", getpid(), c);
            if (i == 1)
                printf("proceso %d: Hay %d minuscula(s)\n", getpid(), c);
            if (i == 2)
                printf("proceso %d: Hay %d espacio(s)\n", getpid(), c);

            write(fd[i+1][1], &TAM, sizeof(int));
            for (int k=0; k<TAM; k++){
                write(fd[i+1][1], &info[k], sizeof(char));
            }

            close(fd[i][0]);
            close(fd[i+1][1]);
        }


    }else{
        close(fd[0][0]);
        for (int k=1; k<npipes; k++){
            if (k != 5) close(fd[k][0]);
            close(fd[k][1]);
        }
        FILE *archivo = fopen("caracteres2.txt", "r");
        if (!archivo) {
            printf("Erro al abrir el archivo\n");
            exit(-1);
        }
        fseek(archivo, 0, SEEK_END);
        int TAM = ftell(archivo);
        fseek(archivo, 0, 0);
        TAM -= 1;
        char info[TAM];

        for (int k=0; k<TAM; k++){
            info[k] = getc(archivo);
        }
        fclose(archivo);

        write(fd[0][1], &TAM, sizeof(int));
        for (int k=0; k<TAM; k++){
            write(fd[0][1], &info[k], sizeof(char));
        }
        close(fd[0][1]);
    }


    if (padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d", getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    } else{
        sleep(3);
    }

    return 0;
}
