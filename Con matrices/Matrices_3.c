#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <time.h>

#define col 20

typedef struct Datos{
    int m;
    int p;
    int n;
    int matrizA[col][col];
    int matrizB[col][col];
    int matrizC[col][col];
}Datos;

void hijo1();
void hijo2();
void padre();

void manejador(){
}

void error(const char* mensaje){
    printf("%s",mensaje);
    exit(1);
}

void matrizImpar(Datos *datos){
    printf("Matriz Impar");
}

void matrizPar(Datos *datos){
    printf("Matriz Par");
}

void llenarMatrices(Datos *datos){
    int i = 0, j = 0;
    for(i = 0;i<datos->m;i++){
        for(j = 0;j<datos->p;j++){
            datos->matrizA[i][j] = rand()%6;
        }
    }

    for(i = 0;i<datos->p;i++){
        for(j = 0;j<datos->n;j++){
            datos->matrizB[i][j] = rand()%6;
        }
    }

    for(i = 0;i<datos->m;i++){
        for(j = 0;j<datos->n;j++){
            datos->matrizC[i][j] = 0;
        }
    }
}

void imprimirMatriz(int matriz[][col],int m,int n){
    int i = 0,j = 0;
    for(i = 0;i<m;i++){
        printf("\n");
        for(j = 0;i<n;j++){
            printf("%d ",matriz[i][j]);
        }
    }
}
int main(){
    srand(time(NULL));
    signal(SIGUSR1,manejador);

    int shmid = shmget(IPC_PRIVATE,sizeof(Datos),IPC_CREAT|0666);
    if(shmid == -1){
        error("No se pudo obtener el shm");
    }
    void *shm_ptr = shmat(shmid,NULL,0);
    if(shm_ptr == (void*)-1){
        error("No se pudo adjuntar el segmento");
    }

    int hijos[2],i = 0;
    Datos *datos = (Datos*)shm_ptr;
    datos->m = 0;
    datos->p = 0;
    datos->n = 0;

    for(i = 0;i<2;i++){
        if(!(hijos[i]=fork())){
            break;
        }
    }

    if(i == 0){
        pause();
        hijo1(datos,hijos);
        shmdt(shm_ptr);

    }else if(i == 1){
        pause();
        hijo2(datos,hijos);
        shmdt(shm_ptr);

    }else if(i == 2){
        usleep(1000);
        padre(datos,hijos);
        shmctl(shmid,IPC_RMID,NULL);
    }

    return 0;
}

void hijo1(Datos *datos,int hijos[]){
    matrizPar(datos);
    kill(getppid(),SIGUSR1);
}

void hijo2(Datos *datos,int hijos[]){
    matrizImpar(datos);
    kill(hijos[0],SIGUSR1);
}

void padre(Datos *datos,int hijos[]){
    int m = 0,n = 0,p = 0;
    printf("Ingrese el valor de: \n");
    printf("M:"); scanf("%d",&m);
    printf("P:"); scanf("%d",&p);
    printf("N:"); scanf("%d",&n);
    datos->m = m;
    datos->p = p;
    datos->n = n;
    llenarMatrices(datos);
    kill(hijos[1],SIGUSR1);
    pause();
}
