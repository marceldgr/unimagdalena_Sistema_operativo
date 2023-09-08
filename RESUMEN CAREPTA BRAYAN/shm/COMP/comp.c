#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>

struct msg{
    int COM, OP, TIME;
}*m;
int main()
{
    int shmid = shmget(IPC_PRIVATE,sizeof(struct msg),0600);
    int COM;
    return 0;
}