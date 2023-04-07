#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
     pid_t f, w;
     int status, x = 171;
     
     printf("Processo principal (PID=%u)\n", getpid());
     for(x=0; x<10; x++){
     	printf("x=%d\n", x);
	}
     f = fork();
     if (f == 0) {
	  sleep(5);
	  printf("Alo do filho (PID=%u)\n", getpid());
	  printf("x=%d\n", x);
	  sleep(3);
	  exit(4);
     } else {
	  printf("Pai (PID=%u) esperando pelo filho (PID=%u)\n", getpid(), f);
	  w = waitpid(f, &status, 0);
	  printf("Status = %d\n", WEXITSTATUS(status));
     }
     return 0;
}
