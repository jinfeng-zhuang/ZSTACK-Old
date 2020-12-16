#ifndef MAIN_H
#define MAIN_H

#define LOG_CONFIG_LENGTH   (256)

enum {
    SX_UNKNOWN = 0,
    SX_AVMIPS,
    SX_ARM
};

struct application {
    struct {
        char log_config[LOG_CONFIG_LENGTH];
        char ip[IP_LENGTH];
        int uart_share;
    } param;
};

#endif