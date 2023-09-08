#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <ctype.h>


int main()
{
    pid_t padre = getpid();
    int i, j, n;
    int c_letras = 0, c_numeros = 0;

    printf("Ingrese el numero de n procesos:");
    scanf("%d", &n);

    pid_t childs[n];
    int fd[n][2];

    for (j=0; j<n; j++) if( pipe(fd[j]) ) exit(-1);
    for (i=0; i<n; i++) if(!(childs[i] = fork())) break;

    if (getpid() != padre){
        for (j=0; j<n; j++){
            if (i != j) close(fd[j][0]);
            if ((i+1) != j) close(fd[j][1]);
        }
        int TAM, nbytes;
        read(fd[i][0], &TAM, sizeof(int));
        read(fd[i][0], &nbytes, sizeof(int));
        char lectura[TAM];

        int np = nbytes/n;

        printf("\nEl proceso %d procesara los caracteres: ", getpid());
        for (int j=0; j<TAM; j++){
            read(fd[i][0], &lectura[j], sizeof(char));

            if (j < np){
                printf("%c", lectura[j]);
                if (isdigit(lectura[j])) c_numeros ++;
                if (isalpha(lectura[j])) c_letras ++;
            }
        }
        printf("\n");

        char lectura2[TAM-np];

        for (int k=np; k<TAM; k++){
            lectura2[k-np] = lectura[k];
        }
        TAM = strlen(lectura2);

        write(fd[i+1][1], &TAM, sizeof(int));
        write(fd[i+1][1], &nbytes, sizeof(int));
        write(fd[i+1][1], lectura2, TAM);
        close(fd[i][0]);
        close(fd[i+1][1]);
    }else{
        close(fd[0][0]);
        for (j=1; j<n; j++){
            close(fd[j][0]);
            close(fd[j][1]);
        }
        FILE *archivo = fopen("caracteres.txt", "r");
        if (!archivo){
            printf("Erro al abrir el archivo\n");
            exit(-1);
        }
        fseek(archivo, 0, SEEK_END);
        int TAM = ftell(archivo);
        fseek(archivo, 0, 0);

        char info[TAM], info2[TAM];
        TAM -=  1;

        int restar = 0, c=0;
        for (int i=0; i<TAM; i++){
            info[i] = getc(archivo);
            if (info[i] != '\n'){
                info2[c] = info[i];
                c++;
            }else{
                restar++;
            }
        }
        fclose(archivo);

        TAM -= restar;
        write(fd[0][1], &TAM, sizeof(int)); //este cambiara a medida que se envia por cada proceso
        write(fd[0][1], &TAM, sizeof(int)); //este se mantendra porque es el numero de bytes del archivo

        for (int k=0; k<TAM; k++){
            write(fd[0][1], &info2[k], sizeof(char));
        }
        close(fd[0][1]);
    }


    if (getpid() != padre)
        printf("El proceso %d leyo %d letra(s) y %d numero(s)\n", getpid(), c_letras, c_numeros);

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        // for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }

    return (0);
}
