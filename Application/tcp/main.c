#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int fd;
    u8  txbuf[128];
    u8  rxbuf[128];
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    network_init();

    fd = tcp_open(app.param.ip, 9902);
    if (fd <= 0) {
        DEBUG("tcp open %s failed\n", app.param.ip);
        goto END;
    }

    memset(rxbuf, 0, sizeof(rxbuf));
    ret = tcp_recv(fd, rxbuf, sizeof(rxbuf), 0);
    if (ret <= 0)
        goto END;

    printf("%s\n", rxbuf);

    while (1) {
        printf("#");
        fflush(stdout);

        memset(txbuf, 0, sizeof(txbuf));
        fgets(txbuf, sizeof(txbuf), stdin);

        ret = tcp_send(fd, txbuf, (u32)strlen(txbuf), 0);
        if (ret <= 0)
            goto END;

        ret = tcp_recv(fd, rxbuf, sizeof(rxbuf), 0);
        if (ret <= 0)
            goto END;

        hexdump(rxbuf, ret, HEXDUMP_ASCII);
    }

END:
    tcp_close(fd);

    network_deinit();

    return 0;
}
