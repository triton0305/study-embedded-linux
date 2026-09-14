#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

typedef struct
{
    int sock;
} Data;

void *send_msg(void *arg);
void *recv_msg(void *arg);

int main(int argc, char *argv[])
{
    int sock;

    struct sockaddr_in serv_adr;

    pthread_t snd_thread;
    pthread_t rcv_thread;

    Data data;

    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        perror("socket() error");
        return 1;
    }

    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock,
                (struct sockaddr *)&serv_adr,
                sizeof(serv_adr)) == -1)
    {
        perror("connect() error");
        close(sock);
        return 1;
    }

    data.sock = sock;

    printf("Connected to server\n");

    if (pthread_create(
            &snd_thread,
            NULL,
            send_msg,
            &data) != 0)
    {
        perror("pthread_create() error");
        close(sock);
        return 1;
    }

    if (pthread_create(
            &rcv_thread,
            NULL,
            recv_msg,
            &data) != 0)
    {
        perror("pthread_create() error");
        close(sock);
        return 1;
    }

    pthread_join(snd_thread, NULL);

    shutdown(sock, SHUT_WR);

    pthread_join(rcv_thread, NULL);

    close(sock);

    return 0;
}

void *send_msg(void *arg)
{
    Data *pdata = (Data *)arg;

    char msg[BUF_SIZE];

    while (1)
    {
        if (fgets(msg, sizeof(msg), stdin) == NULL)
            break;

        if (write(
                pdata->sock,
                msg,
                strlen(msg)) == -1)
        {
            perror("write() error");
            break;
        }
    }

    return NULL;
}

void *recv_msg(void *arg)
{
    Data *pdata = (Data *)arg;

    char msg[BUF_SIZE];

    ssize_t str_len;

    while (1)
    {
        str_len = read(
            pdata->sock,
            msg,
            BUF_SIZE - 1);

        if (str_len == -1)
        {
            perror("read() error");
            break;
        }

        if (str_len == 0)
        {
            printf("Server disconnected\n");
            break;
        }

        msg[str_len] = '\0';

        printf("%s", msg);
    }

    return NULL;
}
