#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <sys/socket.h>

ssize_t writeAll(int fd, const void * buffer, size_t size);

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Usage : ./client <addr> <port>\n");
    return 1;
  }
  
  int  client_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(client_sock == -1)
  {
    perror("socket () error");
    return 1;
  }
  struct sockaddr_in  server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
  
  if(connect(client_sock, (struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)
  {
    perror("connect () error");
    close(client_sock);
    return 1;
  }

  char data[] = "HelloWorldTCP";
  size_t data_size = strlen(data);
  uint32_t net_size = htonl((uint32_t)data_size);
  ssize_t written;
 
//  written = writeAll(client_sock, (char*)&net_size, sizeof(net_size));
  written = writeAll (client_sock, &net_size, sizeof(net_size));
//if(written == -1)    // 시스템 호출 오류만 잡게 된다....
  if(written != (ssize_t)sizeof(net_size))
  {
    close(client_sock);
    return 1;
  }

  written = writeAll (client_sock, data, data_size);
//  if(written==-1)
  if(written != (ssize_t)data_size)
  {
    close(client_sock);
    return 1;
  }

  close(client_sock);
  return 0;
}

ssize_t writeAll(int fd, const void* buffer, size_t size)
{
  size_t total = 0;
  ssize_t write_bytes;
  while (total < size)
  {
    write_bytes = write(fd, (char *) buffer+total, size - total);
    if(write_bytes == -1)
    {
      perror("write () error");
      return -1;
    }
    //size가 다 차지 않았지만 더이상 채워지지 않을 때 
    if(write_bytes == 0)
    {
      return (ssize_t) total;
    }
    total += write_bytes;
  }

  return (ssize_t) total;

}
