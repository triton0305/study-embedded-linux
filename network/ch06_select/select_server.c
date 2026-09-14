#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_SIZE 1024

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    printf("Usage : ./server <port>\n");
    return 1;
  }
  int server_sock;
  int client_sock;

  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if(server_sock == -1)
  {
    perror("Socket () error");
    return 1;
  }

  struct sockaddr_in serv_addr;
//  struct sockaddr_in client_addr;

  memset(&serv_addr , 0 , sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if(bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("bind () error");
    return 1;
  }

  if(listen(server_sock ,5) == -1)
  {
    perror("listen () error");
    return 1;
  }
  
  client_sock = accept(server_sock, NULL, NULL); 
  
  if(client_sock == -1)
  {
    perror("accept () error");
    close(server_sock);
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
    int fd_num = select(nfds,&copy_reads ,NULL, NULL, NULL);
    if(fd_num == -1)
    {
      perror("select () error");
      break;
    }
    if(FD_ISSET(STDIN_FILENO, &copy_reads))
    {
      if(fgets(buffer, MAX_SIZE, stdin)==NULL)
      {
        printf("disconnected\n");
        break;
      }
      if(write(client_sock, buffer , strlen(buffer))==-1)
      {
        perror("write() error");
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
      if(read_bytes ==0)
      {
        printf("peer disconnected\n");
        break;
      }
      buffer[read_bytes] = '\0';
      printf("message : %s", buffer);
    }

  }

  close(client_sock);
  close(server_sock); 
  return 0;
}
