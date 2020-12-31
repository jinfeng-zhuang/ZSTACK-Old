#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define ENABLE_LOG

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#endif

#include <string.h>
#include <stdio.h>

#include <zstack/log.h>

#ifndef _WIN32
#define SOCKET int
#define INVALID_SOCKET (-1)
#define closesocket close
#endif

static SOCKET _socket = INVALID_SOCKET;
static char   _host_name[256];
static int    _port = 0;

int net_disconnect(void)
{
    closesocket(_socket);
    
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

int net_connect(const char* host_name, int port, int msec)
{
    struct sockaddr_in  host_addr;
    int                 opt_val = 1;
    int                 opt_len = sizeof(opt_val);
    
#ifdef _WIN32
    WSADATA ws;
#endif

    memset(&host_addr, 0, sizeof(host_addr));

    host_addr.sin_family = AF_INET;
    
#ifdef _WIN32
    host_addr.sin_addr.S_un.S_addr = inet_addr(host_name);
#else
    inet_pton(AF_INET, host_name, &host_addr.sin_addr);
#endif

    host_addr.sin_port = htons((short)port);

    if (_socket != INVALID_SOCKET)
        closesocket(_socket);

#ifdef _WIN32
    if (0 != WSAStartup(0x0101, &ws)) {
        goto FAILED;
    }
#endif

    _socket = socket(AF_INET, SOCK_STREAM, 0);

    // ��ѡ������Nagle�㷨�����еĶ���˽�ҳ��������ݾͽ���
    // Nagle�㷨 ��Ҷ��������������еȴ�
    // ��Ϸ����ø��㷨������ PK ʱ����û��ʱ˦����������
    setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (char*)& opt_val, opt_len);

    // �������ڼ��Զ��Ƿ�����Ӧ
    // ���Ǹû��Ƽ��ʱ��̫���ˣ�Ĭ�� 2 Сʱ
    setsockopt(_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)& opt_val, opt_len);

    //setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (char*)& msec, sizeof(msec));
    //setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)& msec, sizeof(msec));

    if (-1 == connect(_socket, (struct sockaddr*) & host_addr, sizeof(host_addr))) {
        goto FAILED;
    }

    strcpy(_host_name, host_name);
    _port = port;

    return 0;

FAILED:
    if (_socket != INVALID_SOCKET) {
        closesocket(_socket);
    }

    return -1;
}

int net_transfer(unsigned char* request, void* response)
{
    unsigned int request_len = 0;
    unsigned int response_length = 0;

    unsigned char len[2];
    unsigned char* p = NULL;

    unsigned int count = 0;
    int length = 0;

    if (_socket == INVALID_SOCKET)
        return -1;

    p = (unsigned char*)response;

    request_len = (unsigned short)request[2] + ((unsigned short)request[3] << 8) + 4;

    // send command
    if (send(_socket, (const char*)request, request_len, 0) <= 0) {
        log_warn("send failed\n");
        goto FAILED;
    }

    if (NULL == p) {
        goto NO_RESPONSE;
    }

    // receive data length
    if (recv(_socket, (char*)len, sizeof(len), 0) <= 0) {
        log_warn("recv len failed\n");
        goto FAILED;
    }

    // TODO compare with request data length
    response_length = len[0] + ((int)len[1] << 8);
    count = response_length;

    debug("response len: %d\n", response_length);

    // receive data
    while (count > 0)
    {
        length = recv(_socket, (char*)p, count, 0);

        if (length <= 0) {
            log_warn("recv data failed\n");
            goto FAILED;
        }

        count -= length;
        p += length;
    }

    return response_length;

FAILED:
    if (_socket != INVALID_SOCKET) {
        closesocket(_socket);
    }
    return -1;

NO_RESPONSE:
    return 0;
}

int net_is_connected(void)
{
    if (_socket != INVALID_SOCKET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
