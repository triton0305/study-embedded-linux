#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

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
  ssize_t str_len;
  char buffer[100];
  while(1)
  {
    printf("Input message: ");

    fgets(buffer, sizeof(buffer), stdin);
   
    if(!strcmp(buffer, "q\n")|| !strcmp(buffer, "Q\n"))
    { 
      break;
    }
    write(client_sock, buffer, strlen(buffer));
    str_len = read(client_sock, buffer, sizeof(buffer) -1);
    if(str_len == -1)
    {
      perror("read () error");
      close(client_sock);
      return 1;
    }
    if(str_len == 0)
    {
      printf("server disconnected\n");
      break;
    }

    buffer[str_len] = '\0';
    printf("Message from server: %s", buffer);
    
  }
    

  close(client_sock);
  return 0;
}
