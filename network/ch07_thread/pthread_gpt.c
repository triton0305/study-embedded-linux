#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

typedef struct
{
    int clnt;
    struct sockaddr_in clnt_adr;
} Data;

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];

pthread_mutex_t mutex;

void *handle_clnt(void *arg);
void send_msg(char *msg, int len);

int main(int argc, char *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;

    socklen_t clnt_adr_size;

    pthread_t tid;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1)
    {
        perror("socket() error");
        return 1;
    }

    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock,
             (struct sockaddr *)&serv_adr,
             sizeof(serv_adr)) == -1)
    {
        perror("bind() error");
        close(serv_sock);
        return 1;
    }

    if (listen(serv_sock, 5) == -1)
    {
        perror("listen() error");
        close(serv_sock);
        return 1;
    }

    while (1)
    {
        clnt_adr_size = sizeof(clnt_adr);

        clnt_sock = accept(
            serv_sock,
            (struct sockaddr *)&clnt_adr,
            &clnt_adr_size);

        if (clnt_sock == -1)
        {
            perror("accept() error");
            continue;
        }

        Data *pdata = malloc(sizeof(Data));

        if (pdata == NULL)
        {
            perror("malloc() error");
            close(clnt_sock);
            continue;
        }

        pdata->clnt = clnt_sock;
        pdata->clnt_adr = clnt_adr;

        pthread_mutex_lock(&mutex);

        clnt_socks[clnt_cnt++] = clnt_sock;

        pthread_mutex_unlock(&mutex);

        if (pthread_create(&tid, NULL, handle_clnt, pdata) != 0)
        {
            perror("pthread_create() error");

            pthread_mutex_lock(&mutex);
            clnt_cnt--;
            pthread_mutex_unlock(&mutex);

            close(clnt_sock);
            free(pdata);

            continue;
        }

        pthread_detach(tid);

        printf("Connected client IP : %s\n",
               inet_ntoa(clnt_adr.sin_addr));
    }

    close(serv_sock);

    pthread_mutex_destroy(&mutex);

    return 0;
}

void *handle_clnt(void *arg)
{
    Data *pdata = (Data *)arg;

    char msg[BUF_SIZE];

    ssize_t str_len;

    while ((str_len = read(pdata->clnt, msg, BUF_SIZE)) > 0)
    {
        send_msg(msg, (int)str_len);
    }

    if (str_len == -1)
        perror("read() error");

    pthread_mutex_lock(&mutex);

    for (int i = 0; i < clnt_cnt; i++)
    {
        if (pdata->clnt == clnt_socks[i])
        {
            for (int j = i; j < clnt_cnt - 1; j++)
            {
                clnt_socks[j] = clnt_socks[j + 1];
            }

            clnt_cnt--;

            break;
        }
    }

    pthread_mutex_unlock(&mutex);

    printf("Disconnected client IP : %s\n",
           inet_ntoa(pdata->clnt_adr.sin_addr));

    close(pdata->clnt);

    free(pdata);

    return NULL;
}

void send_msg(char *msg, int len)
{
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < clnt_cnt; i++)
    {
        if (write(clnt_socks[i], msg, len) == -1)
        {
            perror("write() error");
        }
    }

    pthread_mutex_unlock(&mutex);
}
