#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

typedef struct
{
  int client;
  struct sockaddr_in client_addr;
}Data;


void *thread_main(void *arg);

int main(void)
{
  pthread_t tid;

  Data data;
  
  data.client = 5;

  pthread_create(&tid, NULL, thread_main, &data);

  pthread_join(tid, NULL);

  printf("Main end\n");

  return 0;
}

void *thread_main(void *arg)
{
  Data *pdata = (Data *)arg;

  printf("[THREAD] client fd = %d\n", pdata->client);

  return NULL;
}
