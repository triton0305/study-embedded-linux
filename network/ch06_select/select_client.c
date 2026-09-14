#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_SIZE 1024

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
    close(client_sock);
    return 1;
  }

  fd_set reads;
  fd_set copy_reads;

  FD_ZERO(&reads);
  FD_SET(STDIN_FILENO, &reads);
  FD_SET(client_sock, &reads);

  int nfds = client_sock + 1;
  ssize_t read_bytes;
  char buffer[MAX_SIZE];

  
  while(1)
  {
    copy_reads = reads;
    int fd_num = select(nfds,&copy_reads, NULL, NULL, NULL);
    
    if(fd_num==-1)
    {
      perror("select () error");
      break;
    }

    if(FD_ISSET(STDIN_FILENO, &copy_reads))
    {
      if(fgets(buffer,MAX_SIZE, stdin)==NULL)
      {
        printf("disconnected\n");
        break;
      }
      if(write(client_sock, buffer, strlen(buffer))==-1)
      {
        perror("write () error");
        break;
      }
    }

    if(FD_ISSET(client_sock, &copy_reads))
    {
      read_bytes = read(client_sock, buffer, MAX_SIZE - 1);
      if(read_bytes < 0)
      {
        perror("read () error");
        break;
      }
      if(read_bytes == 0)
      {
        printf("peer disconnected\n");
        break;
      }
      buffer[read_bytes] = '\0';
      printf("message : %s",buffer);
    }


  }

  close(client_sock);

  return 0;
}

