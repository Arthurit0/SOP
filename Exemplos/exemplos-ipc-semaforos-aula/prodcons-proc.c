/*
 * Produtor-consumidor com memoria compartilhada e semaforos POSIX.
 * Esta versao coloca produtor e consumidor no mesmo arquivo fonte, 
 * usando fork() para criar um processo filho. O produtor sera' o pai,
 * e o consumidor sera' o filho.
 *
 * Para compilar: gcc -pthread -Wall -o prodcons-proc prodcons-proc.c -lrt
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>

int sai = 0;

int produz()
{
    int n;

    printf("Indique um numero inteiro (0 encerra): ");
    scanf("%d", &n);
    return n;
}

void sair()
{
    sai = 1;
}

int main(int argc, char *argv[])
{
    sem_t *sem_full, *sem_empty;
    int *ptr;
    pid_t pid;
    /* abre objeto de memoria compartilhada, criando se nao existir */
    int fd = shm_open("/shm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (-1 == fd) {
	perror("/shm");
	exit(1);
    }
    /* define o tamanho da regiao de memoria compartilhada */
    int ret = ftruncate(fd, sizeof(int));
    if (ret == -1) {
	perror("/shm");
	exit(2);
    }
    /* obtem um ponteiro para a regiao de memoria compartilhada */
    ptr = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
	perror("shm-mmap");
	exit(3);
    }
    /* cria o semaforo "/full" e o inicializa em 0 */
    sem_full = sem_open("/full", O_CREAT, 0664, 0);
    if (sem_full == SEM_FAILED) {
	perror("full");
	exit(4);
    }
    /* cria o semaforo "/empty" e o inicializa em 1 */
    sem_empty = sem_open("/empty", O_CREAT, 0664, 1);
    if (sem_empty == SEM_FAILED) {
	perror("empty");
	exit(5);
    }
    signal(SIGINT, sair);
    pid = fork();
    if (pid == 0) {   /* filho => consumidor */
	 while (!sai) {
	      if (sem_wait(sem_full) == -1) {   /* => down(&sem_full) + verificacao de erro */
		   perror("sem_wait");
		   break;
	      }
	      printf("Consumindo: %d\n", *ptr);
	      if (sem_post(sem_empty) == -1) {  /* => up(&sem_empty) + verificacao de erro */
		   perror("sem_post");
		   break;
	      }
	      if (*ptr == 0) sai = 1;
	 }
	 exit(0);
    } else {
	 while (!sai) {
	      if (sem_wait(sem_empty) == -1) {  /* => down(&sem_empty) + verificacao de erro */
		   perror("sem_wait");
		   break;
	      }
	      *ptr = produz();
	      printf("'A espera de ser consumido: %d\n\n", *ptr);
	      if (sem_post(sem_full) == -1) {   /* => up(&sem_full) + verificacao de erro */
		   perror("sem_post");
		   break;
	      }
	      if (*ptr == 0) sai = 1;
	 }
    }

    /* 
     * Retirando a chamada `a funcao signal(), este codigo nunca sera' 
     * executado. E' preciso fazer rm /dev/shm/sem.{empty,full} /dev/shm/shm  
     */

    ret = munmap(ptr, sizeof(int));
    if (-1 == ret)
	perror("shm-munmap");
    ret = shm_unlink("/shm");
    if (-1 == ret)
	perror("shm-unlink");
    ret = sem_close(sem_full);
    if (-1 == ret)
	perror("sem_full");
    ret = sem_unlink("/full");
    if (-1 == ret)
	perror("sem_full");
    ret = sem_close(sem_empty);
    if (-1 == ret)
	perror("sem_empty");
    ret = sem_unlink("/empty");
    if (-1 == ret)
	perror("sem_empty");
    return 0;
}
