#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

struct msj{
	int a;
	float b;
};

int main(){

	int fd[2],n;
	pipe(fd);	
	if(fork()==0){
		close(fd[1]);
		struct msj var;
		n=read(fd[0],&var,sizeof(struct msj));
		printf("Leido: int(%d), float(%f)\n", var.a, var.b);
	}else{
		struct msj b;
		close(fd[0]);
		b.a =10;
		b.b=2.3;
		write(fd[1],&b, sizeof(struct msj));
		wait(NULL);
	}
	return 0;
}
