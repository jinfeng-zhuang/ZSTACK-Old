#ifndef MAIN_H
#define MAIN_H

#include <zstack.h>

// TODO it should be defined by dependent API layer
#define BYTE_COUNT_MAX      (256)

enum {
    OPERATION_WRITE,
    OPERATION_READ
};

enum {
    FORMAT_STRUCT,
    FORMAT_BINARY,
    FORMAT_HEXDUMP
};

struct application {
    struct {
        char ip[IP_LENGTH];
        char log_config[LOG_CONFIG_LENGTH];
        unsigned int address;
        unsigned int bytecount;
        unsigned int operation;
        unsigned int format;
        char data[BYTE_COUNT_MAX];
        char datafile[FILE_PATH_MAX];
    } param;

    unsigned char buffer[BYTE_COUNT_MAX];
};

#endif
