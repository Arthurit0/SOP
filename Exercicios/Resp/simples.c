#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *PrintHello(void *arg) {
    long tid = (long)arg;
    printf("Alo da thread %ld\n",
           tid);
    pthread_exit(NULL);
}

void *PrintCube(void *num) {
    long cube = (long)num;
    printf("Cubo de %ld eh %ld\n", cube, cube * cube * cube);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int rc, j = 0, threads_size = 2 * NUM_THREADS + 3;
    long t, n;
    pthread_t threads[threads_size];

    if (argc >= 2) {
        n = atoi(argv[1]);
    }

    for (t = 0; t < threads_size; t++) {
        printf("main: criando thread %ld: PrintHello\n", t);
        rc = pthread_create(&threads[t++], NULL, PrintHello, (void *)t);

        if (rc) {
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }

        printf("main: criando thread %ld: PrintCube\n", t);
        rc = pthread_create(&threads[t], NULL, PrintCube, (void *)n);

        if (rc) {
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }

        if (j < 3) {
            printf("main: criando thread %ld: PrintCube (+%d)\n", ++t, ++j);
            rc = pthread_create(&threads[t], NULL, PrintCube, (void *)n);

            if (rc) {
                printf("ERRO - rc=%d\n", rc);
                exit(-1);
            }
        }
    }
    /* Ultima coisa que main() deve fazer */
    pthread_exit(NULL);
}
