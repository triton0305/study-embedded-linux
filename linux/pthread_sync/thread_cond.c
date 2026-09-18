#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int ready = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{
  pthread_t tid1;
  pthread_t tid2;

  pthread_create(&tid1, NULL, consumer, NULL);
  pthread_create(&tid2, NULL, producer, NULL);
  
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  return 0;
}

void *producer(void *arg)
{
  (void) arg;

  sleep(2);

  pthread_mutex_lock(&mutex);

  ready = 1;

  printf("[PRODUCER] data ready\n");

  pthread_cond_signal(&cond);

  pthread_mutex_unlock(&mutex);

  return NULL;

}

void *consumer(void *arg)
{
  (void) arg;

  pthread_mutex_lock(&mutex);

  while(ready == 0)
  {
    printf("[CONSUMER] waiting...\n");
    pthread_cond_wait(&cond, &mutex);
  }

  printf("[CONSUMER] data received\n");

  pthread_mutex_unlock(&mutex);

  return NULL;

}