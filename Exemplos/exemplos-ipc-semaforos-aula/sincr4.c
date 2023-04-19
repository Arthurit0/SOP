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

sem_t sSO, sOP;

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
     printf("S");
     sem_post(&sSO);
     pthread_exit(NULL);
}

void *O(void *argp)
{
     rand_wait();
     sem_wait(&sSO);
     printf("O");
     sem_post(&sOP);
     pthread_exit(NULL);
}

void *P(void *argp)
{
     rand_wait();
     sem_wait(&sOP);
     printf("P");
     pthread_exit(NULL);
}

int main(void)
{
     pthread_t t1, t2, t3;
     int rc;

     sem_init(&sSO, 0, 0);  /* sSO <- 0 */
     sem_init(&sOP, 0, 0);  /* sOP <- 0 */
     rc = pthread_create(&t1, NULL, S, NULL);   assert(rc == 0);
     rc = pthread_create(&t2, NULL, O, NULL);   assert(rc == 0);
     rc = pthread_create(&t3, NULL, P, NULL);   assert(rc == 0);
     rc = pthread_join(t1, NULL);   assert(rc == 0);
     rc = pthread_join(t2, NULL);   assert(rc == 0);
     rc = pthread_join(t3, NULL);   assert(rc == 0);
     putchar('\n');
     return 0;
}
