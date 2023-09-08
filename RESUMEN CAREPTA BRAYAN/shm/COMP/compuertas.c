#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

struct msg{
int COM, OP, Time;
}*m;

int main(){
int i, shmid = shmget(IPC_PRIVATE, sizeof(struct msg), 0600), COM, root = getpid();
m = shmat(shmid, NULL, 0), m->COM = 5;

for(i = 0; i < 5; i++) if(!fork()) break; //Creacion de los procesos

for(;;)
if(m->COM == i) //Padre o servidor
if(root == getpid()){
printf("\n\t***Servidor***\nMensaje:\n");
scanf("%d %d %d", &COM, &m->OP, &m->Time);

if((COM >= 1 && COM <=5) && (m->OP == 0 || m->OP == 1) && (m->Time >= 0 && m->Time <=2)){
printf("La compuerta %d se %s en %d segundos\n", COM, (m->OP? "abrira": "cerrara"), (m->Time*5));
usleep(m->Time*5*1000000);
m->COM = COM-1;
}else printf("Entrada incorrecta...\n");
}else{ //Hijos o clientes
printf("\n\t---Cliente %d---\nCompuerta %d %s\n",(1+i),(i+1), (m->OP? "abierta": "cerrada"));
m->COM = 5;
}

return 0;
}
