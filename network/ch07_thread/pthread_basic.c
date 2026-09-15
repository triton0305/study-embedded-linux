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

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Usage : ./server <port>\n");
    return 1;
  }
  
  Data data;
  data.client = 5;
  data.client_addr = client_addr;


  pthread_t tid;
  pthread_create(&tid, NULL, thread_main, &data);

  pthread_join(tid, NULL);

  printf("Main end\n");

  return 0;
}

void *thread_main(void *arg)
{
  Data *pdata = (Data *)arg;

  printf("%d\n", pdata->client);
  
  free(pdata);
  return NULL;
}
