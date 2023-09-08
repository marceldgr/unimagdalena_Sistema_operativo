#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(){

    int fd[2], n;
    char Buffer_out[1024];
    char Buffer_in[1024];

    pipe(fd);

    strcpy(Buffer_out, "Hello World Pipes\n");
    write(fd[1], Buffer_out, strlen(Buffer_out));

    n = read(fd[0], Buffer_in, 1024);
    Buffer_in[n] = '\0';
    printf("Leido: %s\n", Buffer_in);

    return 0;

}
