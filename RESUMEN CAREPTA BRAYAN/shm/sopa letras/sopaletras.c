#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <time.h>

size_t tam(int n, size_t tam_elem);
void indexar(char **m, int n, size_t tam_elem);
int buscarPalabra(char palabra[], char** m, int n);
int buscarHorizontal(char palabra[], char** m, int n);
int buscarVertical(char palabra[], char** m, int n);
void invertir(char palabra[]);
void diagonal(char palabra[], char **m, int n);

int main(void) {
  FILE *f = fopen("file.txt","r");
  if(f){
    int n, *turno, Num_caracteres_de_n;
    char *n_digitos = (char*)malloc(sizeof(char)*30);
    fgets(n_digitos,30,f);; //n = 6
    Num_caracteres_de_n = strlen(n_digitos);
    n = atoi(n_digitos);
    printf("%d\n",n);
    //Contar num de palabras a buscar 
    fseek ( f , 43 , SEEK_CUR ); // Posicionar despues de la sopa de letras
    int n_palabras=0;
    char *palabra = (char*)malloc(sizeof(char)*30);
    for(n_palabras=0; !feof(f) ; n_palabras++) fgets(palabra,30,f);
    fseek ( f , (6*7)+Num_caracteres_de_n , SEEK_SET ); // Posicionar despues de la sopa de letras

    // Segmento memoria compartida
    int shmid_m = shmget(IPC_PRIVATE, tam(n,sizeof(char*)), 0600 /*Permisos*/);
    int shmid_t = shmget(IPC_PRIVATE, sizeof(int*), 0600 /*Permisos*/);
    turno = shmat(shmid_t, NULL, 0);
    char **m = shmat(shmid_m, NULL, 0);
    
    //Inicializar
    indexar(m, n, sizeof(char));
    *turno = 0;
    // Fin Segmento memoria compartida
    
    // Creacion de procesos
    int i;
    for(i=0; i<n_palabras-1; i++){
      fgets(palabra,30,f);
      palabra[strlen(palabra)-1]='\0';
      if(!fork()) break;
    }
    if( i == n_palabras-1){//proceso padre

      //Llenar la matriz de sopa de letra
      fseek(f, Num_caracteres_de_n, SEEK_SET); 
      for(int j=0; j<n; j++){
       int c=0;
       while((m[j][c]=fgetc(f))!='\n') c++;
      }
      diagonal(palabra,m,n);
      *turno = 1;
      for(int k=0; k<n_palabras-1; k++) wait(NULL);
      
    }else{ // PROCESOS HIJOS
        
        while( *turno == 0); // Esperar mientras sea cargada la sopa de letra en shm
        printf("Proceso %d -> Buscar(%s): R/ ", i,palabra);
      
         if( buscarPalabra(palabra, m, n) >= 1 ){
          printf("ENCONTRADO <--\n");
          }else{
            printf("NO ENCONTRADO \n");
          }
          
        /*
        if( buscarPalabra(palabra, m) == 1 ){
          printf("ENCONTRE LA PALABRA %s", palabra);
        }
        invertir(palabra);
        if( buscarPalabra(palabra, m) == 1 ){
          printf("ENCONTRE LA PALABRA %s", palabra);
        }*/
    }

  }
  return 0;
}

size_t tam(int n, size_t tam_elem){
    size_t tamFila =  (n * tam_elem);
    size_t total = n * (sizeof(void *)) * tamFila;
    return total;
}

void indexar(char **m, int n, size_t tam_elem){
    size_t tamFila =  (n * tam_elem);
    m[0] = (char*)(m + tamFila);
    for(int i = 1; i < n; i++)
        m[i] = (m[i-1]+tamFila);
}

int buscarPalabra(char palabra[], char** m, int n){
  int res = buscarHorizontal(palabra, m, n);
  if( res != 1){
    res = buscarVertical(palabra, m, n);
    if(res != 1){
      invertir(palabra);
      res = buscarHorizontal(palabra, m, n);
      if( res != 1){
        res = buscarVertical(palabra, m, n);
      }
    }
  }
  return res;
  
}
int buscarHorizontal(char palabra[], char** m, int n){
  int res = 0;
  for(int g=0; g<n; g++){
    if(strstr(m[g], palabra)!=NULL){
      res=1;
      break;
    }
  }
  return res;
}

int buscarVertical(char palabra[], char** m, int n){
  int res=0;
  char vertical[strlen(palabra)];
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      vertical[j] = m[j][i];
    }
    if(strstr(vertical,palabra)!=NULL){
      res=1;
      break;
    }
  }
  return res;
}
void invertir(char palabra[]){
  int k = strlen(palabra), s=0;
  char aux[k];
  
  for(int c=k-1; c>-1; c--){
    aux[s] = palabra[c];
    s++;
  }
  strcpy(palabra,aux);
}

void diagonal(char palabra[], char **m, int n){
  for (int i = 0; i < n; i++) {
    for (int j = 0; j<n; j++) {
          printf("%c ",m[i][j]);
        }
    printf("\n");
}
printf("--------------------\n");
  for (int i = 0; i < n; i++) {
    for (int j = n-i; j<n; j++) {
          printf("%c ",m[j][i]);
        }
    printf("\n");
}
printf("\n");
}