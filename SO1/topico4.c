#include <pthread.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_THREADS 3
sem_t sem_B, sem_C;

void *threadBody (void *id)
{
  long tid = (long) id ; // ID da thread

  printf ("t%02ld: Olá!\n", tid);
  sleep (3);   
  if (tid == 0) {
    sem_post(&sem_B);
    printf ("A");
  }
  if (tid == 1) {
    sem_wait(&sem_B);
    printf("B");
    sem_post(&sem_C);
  }
  if (tid == 2) {
    sem_wait(&sem_C);
    printf ("C");
  }
  pthread_exit (NULL);
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS];
  long i, status;

  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i);
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i);
    if (status)
    {
      perror ("pthread_create");
      exit (1);
    }
  }
  
  printf ("Main: fim\n");
  pthread_exit (NULL);
}
