/* Programa para mostrar sincronizacao de threads. O programa deveria
 *  imprimir "SOP", mas isso nem sempre ocorre.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

pthread_cond_t condSO = PTHREAD_COND_INITIALIZER;
pthread_cond_t condOP = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int letras = 0;

void rand_wait(void)
{
#define RMAX 1000000
     long r;
     while ((r = random() % RMAX) != 171)
	  ;
}

void *S(void *argp)
{
     rand_wait();
     pthread_mutex_lock(&mtx);
     printf("S");
     letras++;
     pthread_cond_signal(&condSO);
     pthread_mutex_unlock(&mtx);
     pthread_exit(NULL);
}

void *O(void *argp)
{
     rand_wait();
     pthread_mutex_lock(&mtx);
     while (letras < 1) {
	  fprintf(stderr, "O entrando em cond_wait\n");
	  pthread_cond_wait(&condSO, &mtx);
     }
     printf("O");
     letras++;
     pthread_cond_signal(&condOP);
     pthread_mutex_unlock(&mtx);
     pthread_exit(NULL);
}

void *P(void *argp)
{
     rand_wait();
     pthread_mutex_lock(&mtx);
     while (letras < 2) {
	  fprintf(stderr, "P entrando em cond_wait\n");
	  pthread_cond_wait(&condOP, &mtx);
     }
     printf("P");
     pthread_mutex_unlock(&mtx);
     pthread_exit(NULL);
}

int main(void)
{
     pthread_t t1, t2, t3;
     int rc;
     
     rc = pthread_create(&t1, NULL, S, NULL);   assert(rc == 0);
     rc = pthread_create(&t2, NULL, O, NULL);   assert(rc == 0);
     rc = pthread_create(&t3, NULL, P, NULL);   assert(rc == 0);
     rc = pthread_join(t1, NULL);   assert(rc == 0);
     rc = pthread_join(t2, NULL);   assert(rc == 0);
     rc = pthread_join(t3, NULL);   assert(rc == 0);
     putchar('\n');
     return 0;
}
