#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define NELEM 80000000

long v[NELEM];

void *busca_inc(void *arg) {
     long i, ret = -1, n = (long)arg;

     for (i = 0; i < NELEM; i++) {
	  if (v[i] == n) {
	       ret = i;
	       break;
	  }
     }
     pthread_exit((void *)ret);
}

int main(int argc, char *argv[])
{
     long i, n;
     pthread_t thr;
     int rc;
     void *status;

     if (argc < 2) {
	  fprintf(stderr, "e o n, querido?\n");
	  exit(1);
     }
     n = atoi(argv[1]);
     for (i = 0; i < NELEM; i++) 
	  v[i] = i+1;
     //i = busca_inc(n);
     rc = pthread_create(&thr, NULL, busca_inc, (void *)n);  assert(rc == 0);
     rc = pthread_join(thr, &status);                        assert(rc == 0);
     i = (long)status;
     if (i >= 0)
	  printf("O numero %ld esta' na posicao %ld do vetor\n", n, i);
     else
	  printf("O numero %ld nao foi encontrado no vetor\n", n);
     return 0;
}
