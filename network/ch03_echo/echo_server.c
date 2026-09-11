#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Usage : ./server <port> <file>\n");
    return 1;
  }
  
  int server_sock;
  int client_sock;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);

  if(server_sock == -1)
  {
    perror("socket () error");
    return 1;
  }

  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(atoi(argv[1]));

  if (bind (server_sock , (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("bind () error");
    close(server_sock);
    return 1;
  }

  if (listen ( server_sock, 5) == -1 )
  {
    perror("listen () error");
    close(server_sock);
    return 1;
  }

  client_sock = accept(server_sock, NULL, NULL); // 원래는 (struct sockaddr*), (socklen_t*) 
  if(client_sock ==  -1 )
  {
    perror("accept () error");
    close(server_sock);
    return 1;
  }

  char buffer[100];
  ssize_t  str_len;
  str_len  = read (client_sock, buffer, sizeof(buffer)-1);
  

  if(str_len == -1)
  {
    perror("read () error");
    close(client_sock);
    close(server_sock);
    return 1;
  }
  if(str_len == 0)
  {
    printf("client disconnected\n");
    close(client_sock);
    close(server_sock);
    return 0;
  }

  buffer[str_len] = '\0';
  printf("received: %s\n", buffer);
  write(client_sock , buffer , str_len);
  close(client_sock);
  close(server_sock);
  return 0;
}
