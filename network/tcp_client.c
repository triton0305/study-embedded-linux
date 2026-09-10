#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
  printf("Usage : ./client <IP> <port>\n");
  return 1;
  }
  
  int client_sock;

  client_sock = socket(AF_INET, SOCK_STREAM, 0);

  if(client_sock == -1)
  {
    perror("socket () error");
    return 1;
  }
  
  struct sockaddr_in serv_addr; 
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    
  if(connect(client_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("connect () error");
    return 1;
  }

  char message[] = "Hello Server";
  write(client_sock, message, strlen(message));

  close(client_sock);

  return 0;
}

