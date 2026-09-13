#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_DATA_SIZE 1024

ssize_t readAll(int fd, void *buffer, size_t size);

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Usage : ./stream_server <port>\n");
    return 1;
  }
  int server_sock;
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(server_sock == -1)
  {
    perror("socket () error");
    return 1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[1]));
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("bind () error");
    close(server_sock);
    return 1;
  }
  if(listen(server_sock, 5) == -1)
  {
    perror("listen () error");
    close(server_sock);
    return 1;
  } 

  int client_sock = accept(server_sock, NULL, NULL);
  if(client_sock == -1)
  {
    perror("accept () error");
    close(server_sock);
    return 1;
  }

  uint32_t net_size;

  ssize_t str_len = readAll(client_sock, &net_size, sizeof(net_size));
  if(str_len != (ssize_t)sizeof(net_size))
  {
    fprintf(stderr,"Failed to receive data size\n");
    close(client_sock);
    close(server_sock);
    return 1;
  }

  uint32_t data_size = ntohl(net_size);
  printf("Data size: %u bytes\n", (unsigned int)data_size);

  if(data_size == 0 || data_size > MAX_DATA_SIZE)
  {
    fprintf(stderr, "Invalid data size: %u bytes\n", (unsigned int)data_size);
    close(client_sock);
    close(server_sock);
    return 1;
  }

  char buffer[MAX_DATA_SIZE];

  str_len = readAll(client_sock, buffer, data_size);
  if(str_len == -1)
  {
    close(server_sock);
    close(client_sock);
    return 1;
  }
  if((uint32_t)str_len != data_size)
  {
    printf("incomplete data: received %zd/%u bytes\n", str_len,(unsigned int) data_size);
    fwrite(buffer, 1,(size_t) str_len, stdout);
    printf("\n");

    close(client_sock);
    close(server_sock);
    return 1;
  }
  printf("received: %zd/%u bytes\n", str_len, (unsigned int)data_size);

  printf("Complete data: ");
  fwrite(buffer, 1, (size_t)str_len, stdout);
  printf("\n");

  close(client_sock);
  close(server_sock);
  return 0;
}

ssize_t readAll(int fd, void* buffer, size_t size)
{
  size_t total = 0;
  ssize_t read_bytes;
  while(total < size)
  {
    read_bytes = read(fd,(char*) buffer+total,size-total);
    if(read_bytes == -1)
    {
      perror("read () error");
      return -1;
    }
    if(read_bytes  == 0)
    {
      return (ssize_t)total;
    }
    total += read_bytes; 
  }

  return (ssize_t)total; 
}

