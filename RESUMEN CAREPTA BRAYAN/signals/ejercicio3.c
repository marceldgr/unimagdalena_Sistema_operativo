#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void handler(){}

int main(){
  signal(SIGUSR1, handler);
  pid_t pids[3], main_process = getpid();
  int i;

  //Creating the hierarchy
  for(i = 2; i >= 0; i--)
    if(!(pids[i] = fork())){
      if(i != 1) pids[i] = fork();
      break;
    }

  //Has been programmed the following process: 3, 5
  if(main_process == getpid()){
    //Main process (3)
    sleep(2);
    printf("Padre: %d\n", getpid());
    kill(pids[0], SIGUSR1);
    pause();
    printf("Padre: %d\n", getpid());
  }else{
    //Children processes
    pause();
    printf("Hijo: %d\n", getpid());

    if(getppid() == main_process){

      if(i == 1) kill(pids[i+1], SIGUSR1); //Process 5
      else{ //Process: 4 and 6
        kill(pids[i], SIGUSR1);
        pause();
        printf("Hijo: %d\n", getpid());
        if(i == 0) kill(pids[i+1], SIGUSR1);
        else kill(getppid(), SIGUSR1);
      }
    }else kill(getppid(), SIGUSR1); //processes 7 and 8
  }
  if (getpid() == main_process) {
      char a[50];
      sprintf(a,"pstree -lp %d",getpid());
      system(a);
  }else sleep(4);
  return 0;
}
