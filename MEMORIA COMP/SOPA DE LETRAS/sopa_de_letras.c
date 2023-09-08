#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * sizeof (void *); // index size
    size += (cols * rows * sizeElement); // Data size
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    size_t sizeRow = cols * sizeElement;
    m[0] = m + rows;
    for (int i=1; i<rows; i++){
        m[i] = (m[i-1] + sizeRow);
    }
}

void print_matrix(char **m, int rows, int cols){
    for (int r = 0; r < rows; r++){
        for (int c = 0; c < cols; c++){
            printf("%c\t", m[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

int *buscarPalabra(char **m, char *palabra, int n){
    int ncaracteres = strlen(palabra), fletra, cletra;
    int cont_letras, *result = malloc(4*sizeof(int));

    for (int a=0; a<n; a++){
        for (int b=0; b<n; b++){
            if (m[a][b] == palabra[0]){

                fletra = a;   cletra = b;  cont_letras = 0;
                // si esta de izq a derechar
                if ((n-cletra) >= ncaracteres){
                    for (int d=0; d<ncaracteres; d++){
                        if (palabra[d] == m[a][cletra]){
                            cont_letras++;
                            cletra++;
                        }else break;
                    }
                    if (cont_letras ==  ncaracteres){
                        result[0] = 1; //direccion
                        result[1] = a; //fila
                        result[2] = b; //columna inicio
                        result[3] = (b+ncaracteres)-1; //columna fin
                        return result;
                    }
                }

                fletra = a;  cletra = b;  cont_letras=0;
                //si es posible que este de der a izq
                if ((cletra+1) >= ncaracteres){
                    for (int d=0; d<ncaracteres; d++){
                        if (palabra[d] == m[a][cletra]){
                            cont_letras++;
                            cletra--;
                        }else break;
                    }
                    if (cont_letras ==  ncaracteres){
                        result[0] = 2; //direccion
                        result[1] = a; //fila
                        result[2] = b; //columna inicio
                        result[3] = (b+1)-ncaracteres; //columna fin
                        return result;
                    }
                }

                fletra = a;  cletra = b;  cont_letras=0;
                // si esta de arriba hacia abajo
                if ((fletra+ncaracteres)-1 <= n){
                    for (int d=0; d<ncaracteres; d++){
                        if (palabra[d] == m[fletra][b]){
                            cont_letras++;
                            fletra++;
                        }else break;
                    }
                    if (cont_letras ==  ncaracteres){
                        result[0] = 3; //direccion
                        result[1] = b; //columna
                        result[2] = a; //fila inicio
                        result[3] = a+ncaracteres-1; //fila fin
                        return result;
                    }
                }

                fletra = a;  cletra = b;  cont_letras=0;
                // si esta de abajo hacia arriba
                if ((fletra-ncaracteres)+1 >= 0){
                    for (int d=0; d<ncaracteres; d++){
                        if (palabra[d] == m[fletra][b]){
                            cont_letras++;
                            fletra--;
                        }else break;
                    }
                    if (cont_letras ==  ncaracteres){
                        result[0] = 4; //direccion
                        result[1] = b; //columna
                        result[2] = a; //fila inicio
                        result[3] = (a-ncaracteres)+1; //fila fin
                        return result;
                    }
                }
            }
        }
    }
    result[0] = -1;
    return result;
}

int main(){
    system("clear");
    int i, n, shmid_turno, *turno;
    int shmid_m, rows, cols;
    char **m;

    shmid_turno = shmget(IPC_PRIVATE, sizeof(int), 0600);
    turno = shmat(shmid_turno, 0, 0);

    FILE *f = fopen("sopa_de_letras.txt", "r");
    if (!f) exit(-1);
    fscanf(f, "%d", &n);
    rows = n; cols = n;

    size_t sizeMatrix = sizeof_dm(rows, cols, sizeof(char));
    shmid_m = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);
    m = shmat(shmid_m, NULL, 0);
    create_index((void*)m, rows, cols, sizeof(char));

    int pos_inicio_matriz = ftell(f)+1;
    int pos_inicio_palabras = ((n*2)*n)+pos_inicio_matriz;
    fseek(f, pos_inicio_palabras, 0); //posicionar puntero despues de la matriz

    char linea[n+1], palabras[n+1][n+1];
    int n_palabras = 0, len;
    while(!feof(f)){
        fgets(linea, n+2, f);
        strcpy(palabras[n_palabras], linea);
        len = strlen(palabras[n_palabras])-1;
        palabras[n_palabras][len] = '\0';
        n_palabras++;
    }
    n_palabras--; //eliminar la repeticion de la ultima linea del archivo

    *turno = n_palabras;
    pid_t root = getpid();
    for (i=0; i<n_palabras; i++) if (!fork()) break;

    if(root == getpid()){
        char b[500];
        sprintf(b,"pstree -lp %d",getpid());
        system(b);
    }else sleep(3);

    if (i == n_palabras){ // padre
        printf("Padre [%d]: Matriz cargada a memoria.\n", getpid());
        fseek(f, pos_inicio_matriz, 0); //reposicionamos el cursor
        char caracter;
        int cont=0, fila=0;
        for (int a=0; a<pos_inicio_palabras-2; a++){
            caracter = fgetc(f);
            if (caracter != ' ' && caracter != '\n'){
                m[fila][cont] = caracter;
                cont++;
            }
            if (cont == n) {
                fila++;
                cont=0;
            }
        }

        print_matrix(m, n, n);

        fclose(f);
        *turno = 0;
        for (int a=0; a<n_palabras; a++) wait(NULL);
    }else{
        while (i != *turno) sleep(0.5);
        printf("Hijo %d [%d]: Buscando la palabra -> %s\n", i+1, getpid(), palabras[i]);

        int *posiciones = malloc(4*sizeof(int));
        posiciones = buscarPalabra(m, palabras[i], n);

        switch (posiciones[0]) {
            case 1:
                printf("  Encontrado: La palabra esta de izquierda a derecha, en la fila %d desde "
                "la columna %d hasta la columna %d\n", posiciones[1], posiciones[2], posiciones[3]);
                break;
            case 2:
                printf("  Encontrado: La palabra esta de derecha a izquierda, en la fila %d desde "
                "la columna %d hasta la columna %d\n", posiciones[1], posiciones[2], posiciones[3]);
                break;
            case 3:
                printf("  Encontrado: La palabra esta de arriba hacia abajo, en la columna %d desde "
                "la fila %d hasta la fila %d\n", posiciones[1], posiciones[2], posiciones[3]);
                break;
            case 4:
                printf("  Encontrado: La palabra esta de abajo hacia arriba, en la columna %d desde "
                "la fila %d hasta la fila %d\n", posiciones[1], posiciones[2], posiciones[3]);
                break;
            default:
                printf("  Palabra no encontrada.\n"); break;
        }
        printf("\n");

        if (i < n_palabras-1) *turno += 1;
    }

    return 0;
}
