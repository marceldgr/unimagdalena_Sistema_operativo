#include <stdio.h> //IO
#include <stdlib.h> //Funciones del sistema
#include <unistd.h>

int main(){
    int fd[2][2], i, pids[2];
    for(i=0; i < 2; i++) if(pipe(fd[i]) != 0) exit(-1);
    for(i=0; i < 2; i++) if((pids[i] = fork()) == 0) break;
    
    if(i == 1){
        close(fd[1][0]);
        close(fd[1][1]);
        close(fd[0][0]);
        
        FILE *f = fopen("prueba.txt", "r");
        if(f == NULL){
            perror("Error de apertura!");
            return -1;
        }
        
        fseek(f, 0, 2);
        int TAM = ftell(f);
        fseek(f, 0, 0);
        
        char info[TAM];
        for(i = 0; i < TAM; i++) info[i] = getc(f);
        
        write(fd[0][1], &TAM, sizeof(int));
        write(fd[0][1], info, TAM);
        fclose(f);
        close(fd[0][1]);
    }else if(i == 0){
        close(fd[1][0]);
        close(fd[0][1]);
        
        int T, n_vocales = 0;
        read(fd[0][0], &T, sizeof(int));
        char info[T];
        read(fd[0][0], info, T);
        
        for(i = 0; i < T; i++){
            switch(info[i]){
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                case 'A':
                case 'E':
                case 'I':
                case 'U':
                case 'O':
                    n_vocales++;
                    break;
            }
        }
        
        write(fd[1][1], &T, sizeof(int));
        write(fd[1][1], info, T);
        write(fd[1][1], &n_vocales, sizeof(int));
        close(fd[1][1]);
        close(fd[0][0]);
    }else{//Proceso padre
        close(fd[1][1]);
        close(fd[0][1]);
        close(fd[0][0]);
        int T, n_vocales, n_saltos = 0;
        read(fd[1][0], &T, sizeof(int));
        char info[T];
        read(fd[1][0], info, T);
        read(fd[1][0], &n_vocales, sizeof(int));
        
        for(i = 0; i < T; i++) if(info[i] == '\n') n_saltos++;
        
        printf("# vocales: %d\n# de saltos %d\n", n_vocales, n_saltos);
        close(fd[1][0]);
    }
    
}











