#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>

#include <stdio.h>
#include <string.h>
#include <zstack/log.h>
#include <zstack/net.h>
#include <zstack/types.h>

int network_init(void) {
    WSADATA ws;

    if (0 != WSAStartup(0x0101, &ws)) {
        return -1;
    }

    return 0;
}

void network_deinit(void) {
    WSACleanup();
}

int tcp_open(const char* hostname, u16 port) {
    
    int     fd;
    int     opt_val = 1;
    int     opt_len = sizeof(opt_val);
    struct sockaddr_in host_addr;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, opt_len);
    setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt_val, opt_len);

    memset(&host_addr, 0, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    host_addr.sin_addr.S_un.S_addr = inet_addr(hostname);
    host_addr.sin_port = htons((short)port);

    if (-1 == connect(fd, (struct sockaddr*)&host_addr, sizeof(host_addr))) {
        return -1;
    }

    return fd;
}

int tcp_close(int fd) {
    if (fd > 0)
        closesocket(fd);

    return 0;
}

int tcp_send(int fd, u8* buf, s32 len, s32 flags) {
    return send(fd, buf, len, flags);
}

int tcp_recv(int fd, u8* buf, s32 len, s32 flags) {
    return recv(fd, buf, len, flags);
}
