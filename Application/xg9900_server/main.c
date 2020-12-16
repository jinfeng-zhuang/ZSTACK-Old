#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define VERSION ("0.0.1")

#define portnumber 9900

unsigned char recv_buffer[4096];

// short connection
void *client_thread(void *arg)
{
    int fd = (int)arg;
    int iDataNum;

    iDataNum = recv(fd, recv_buffer, 4096, 0);
    if (iDataNum < 0) {
        goto CLIENT_DISCONNECT;
    }
    else if (iDataNum == 0) {
        log2buffer("[%d] disconnected by client\n", fd);
        goto CLIENT_DISCONNECT;
    }
    else {
        log2buffer("[%d] recv %d bytes\n", fd, iDataNum);
        goto CLIENT_DISCONNECT;
    }

CLIENT_DISCONNECT:
    client_thread_end();

    close(fd);
}

int server_fd;

void signal_proc(int signo)
{
    log2buffer("signo %d arrived\n", signo);

    close(server_fd);

    sleep(3);

    _exit(0);
}

int main(int argc, char* argv[])
{
    int new_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    pthread_t pid;

    log2buffer("XG9900 Server %s\n", VERSION);

    log_thread_init();
    client_thread_int();

    signal(SIGINT, signal_proc);
    signal(SIGTERM, signal_proc);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return 0;
    }
    
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(portnumber);
    if (bind(server_fd, (struct sockaddr*)(&server_addr), sizeof(struct sockaddr)) == -1) {
        log2buffer("port %d already used\n", portnumber);
        goto SERVER_ERROR;
    }
    
    if (listen(server_fd, 5) == -1) {
        log2buffer("Listen error:%s\n\a", strerror(errno));
        goto SERVER_ERROR;
    }
    
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        new_fd = accept(server_fd, (struct sockaddr*)(&client_addr), &sin_size);
        if (new_fd == -1) {
            log2buffer("Accept error:%s\n\a", strerror(errno));
            goto SERVER_ERROR;
        }
        else {
            if (0 == client_thread_start(client_thread, new_fd)) {
                log2buffer("New connection[%d]: %s\n", new_fd, inet_ntoa(client_addr.sin_addr));
            }
            else {
                log2buffer("New connection: but thread number out of range, defuse\n");
            }
        }
    }

SERVER_ERROR:

    close(server_fd);
    
    return 0;
}

