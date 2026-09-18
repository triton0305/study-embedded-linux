#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define QUEUE_SIZE 5
#define DATA_COUNT 10
typedef struct
{
  int queue[QUEUE_SIZE];

  int front;
  int rear;
  int count;
  bool done;

  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
}Data;


void dataInit(Data *data);
void *producer(void *arg);
void *consumer(void *arg);

int main(void)
{

  Data data;

  dataInit(&data);

  pthread_t tid1;
  pthread_t tid2;

  pthread_create(&tid1, NULL, consumer, &data);
  pthread_create(&tid2, NULL, producer, &data);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&data.mutex);
  pthread_cond_destroy(&data.not_empty);
  pthread_cond_destroy(&data.not_full);

  printf("Main_end\n");

  return 0;
}

void dataInit(Data *data)
{
  //Data* data = (Data*) arg;

  memset(data->queue, 0, sizeof(data->queue));

  data->count = 0;
  data->front = 0;
  data->rear = 0;
  data->done = false;
  
  pthread_mutex_init(&data->mutex, NULL);
  pthread_cond_init(&data->not_empty, NULL);
  pthread_cond_init(&data->not_full, NULL);
}

void *producer(void *arg)
{
  Data *data = (Data *)arg;

  for(int i = 1; i <= DATA_COUNT ;i++)
  {
    pthread_mutex_lock(&data->mutex);

    while(data->count == QUEUE_SIZE)
    {
      printf("[PRODUCER] Queue Full - waiting...\n");
      pthread_cond_wait(&data->not_full, &data->mutex);
    }

    data->queue[data->rear] = i;
    data->rear = (data->rear + 1) % QUEUE_SIZE;
    data->count++;

    printf("[PRODUCER] push %d, count = %d\n",i,data->count);

    pthread_cond_signal(&data->not_empty);

    pthread_mutex_unlock(&data->mutex);

    usleep(100000);

  }

  pthread_mutex_lock(&data->mutex);

  data->done = true;

  pthread_cond_signal(&data->not_empty);

  pthread_mutex_unlock(&data->mutex);
  
  return NULL;

}

void *consumer(void *arg)
{
  Data *data = (Data *)arg;

  while(1)
  {
    pthread_mutex_lock(&data->mutex);
    
    while(data->count == 0 && data->done ==false)
    {
      printf("[CONSUMER] Queue Empty - waiting...\n");
      pthread_cond_wait(&data->not_empty, &data->mutex);
    }

    if(data->count == 0 && data->done == true)
    {
      pthread_mutex_unlock(&data->mutex);
      break;
    }

    int temp = data->queue[data->front];

    data->front = (data->front + 1) % QUEUE_SIZE;
    data->count--;
    printf("[CONSUMER] pop %d, count = %d\n", temp, data->count);

    pthread_cond_signal(&data->not_full);

    pthread_mutex_unlock(&data->mutex);
    
    usleep(300000);

  }
  printf("[CONSUMER] finished\n");

  return NULL;


}