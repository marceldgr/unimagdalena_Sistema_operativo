#include <stdio.h>
#include <stdlib.h>




int main(int argc, char const *argv[]) {

    FILE * flujo = fopen("prueba.txt", "r");

    if(!flujo){
        perror("Error en el momento de cargar el archivo");
        return 1;
    }else{
        char caracter;
        while(feof(flujo) == 0){//mientras no se termi
            caracter = fgetc(flujo);
            printf("%s\n",caracter );
        }
        printf("\n Se ha ñ leido el archivo correctamente");
        return 0;
    }
}
/*
    int i, j = -1, n;
    pid_t childs[3], padre = getpid();

    for ( i = 0; i < 3; i++) {
        if (!(childs[i] = fork())) {
            if( i == 2){
                for ( j = 0; j < 2; j++) {
                    if (childs[j] = fork()) {
                        break;
                    }
                }
            }
            break;
        }
    }

    if (getpid() == padre) {

    }else{
        if (i == 0) {

        }
    }

    if(padre==getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
        for( i = 0; i < 3; i++) wait(NULL);
    }else{
        sleep(3);
    }
    return 0;
}

    */
