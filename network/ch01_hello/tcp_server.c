#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include <arpa/inet.h>
#include <sys/socket.h>

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
  struct sockaddr_in client_addr;

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
    return 1;
  }
  char buffer[100];
  int str_len;
  str_len = read(client_sock,buffer, sizeof(buffer)-1);
  if(str_len == -1)
  {
    perror("read () error");
    return 1;
  }

  if(str_len == 0)
  {
    printf("Client disconnected\n");
    close(client_sock);
  }
  buffer[str_len] = '\0';

  printf("Received : %s\n",buffer);

  write(client_sock, buffer, str_len);

  close(client_sock);
  close(server_sock); 
  return 0;
}
