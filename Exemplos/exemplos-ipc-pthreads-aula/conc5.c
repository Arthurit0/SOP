/* Exercicio 6 da lista de pthreads */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX 2000

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
long n;

void *f1(void *argp)
{
    long i;
    for (i = 0; i < MAX; i++) {
	 pthread_mutex_lock(&m);
	 n++; /* r1 <- n ; add r1, 1 ; n <- r1 */
	 pthread_mutex_unlock(&m);
    }
    pthread_exit(NULL);
}

void *f2(void *argp)
{
    long i;
    for (i = 0; i < MAX; i++) {
	 pthread_mutex_lock(&m);
	 n--;
	 pthread_mutex_unlock(&m);
    }
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t t1, t2;
    int rc;

    n = 0;
    rc = pthread_create(&t1, NULL, f1, NULL);   assert(rc == 0);
    rc = pthread_create(&t2, NULL, f2, NULL);   assert(rc == 0);
    rc = pthread_join(t1, NULL);   assert(rc == 0);
    rc = pthread_join(t2, NULL);   assert(rc == 0);
    printf("%ld\n", n);
    return 0;
}
