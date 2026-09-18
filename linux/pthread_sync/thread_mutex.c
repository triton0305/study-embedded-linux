#include <stdio.h>
#include <pthread.h>

#define LOOP 1000000

int count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadMain(void *arg);

int main(void)
{
  //pthread_mutex_init(&mutex, NULL);

  pthread_t tid1;
  pthread_t tid2;

  pthread_create(&tid1, NULL, threadMain, NULL);
  pthread_create(&tid2, NULL, threadMain, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("count : %d\n", count);

  //pthread_mutex_destroy(&mutex);

  return 0;
}

void *threadMain(void *arg)
{
  (void) arg;

  for (int i = 0 ; i < LOOP; i++)
  {
    pthread_mutex_lock(&mutex);
    count++;
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}
