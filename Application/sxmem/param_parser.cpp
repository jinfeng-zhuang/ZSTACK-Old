#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,

    OPTION_ADDRESS,
    OPTION_BYTECOUNT,
    OPTION_OPERATION,
    OPTION_FORMAT,
    OPTION_DATA
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},

    {"address", required_argument, 0, OPTION_ADDRESS},
    {"bytecount", required_argument, 0, OPTION_BYTECOUNT},
    {"operation", required_argument, 0, OPTION_OPERATION},
    {"format", required_argument, 0, OPTION_FORMAT},
    {"data", required_argument, 0, OPTION_DATA},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;

    if (argc <= 1) {
        return -1;
    }

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            log(LOG_USER, "Version: %s\n", version);
            break;
        case OPTION_HELP:
            log(LOG_USER, usage);
            break;
        case OPTION_LOG:
            if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
                log(LOG_USER, "log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
                return -1;
            }
            strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
            break;
        case OPTION_ADDRESS:
            sscanf(optarg, "%x", &app->param.address);
            break;
        case OPTION_BYTECOUNT:
            sscanf(optarg, "%d", &app->param.bytecount);
            break;
        case OPTION_OPERATION:
            if (0 == strcmp("write", optarg)) {
                app->param.operation = OPERATION_WRITE;
            }
            else if (0 == strcmp("read", optarg)) {
                app->param.operation = OPERATION_READ;
            }
            else {
                return -1;
            }
            break;
        case OPTION_DATA:
            // should reserve 1 byte for '\0'
            if (strlen(optarg) >= BYTE_COUNT_MAX)
                return -1;
            memcpy(app->param.data, optarg, strlen(optarg));
            break;
        default:
            return -1;
        }
    }

    if (optind >= argc)
        return -1;

    if (strlen(argv[optind]) >= IP_LENGTH)
        return -1;
    memcpy(app->param.ip, argv[optind], strlen(argv[optind]));

    if ((app->param.ip[0] == '\0') || (app->param.address == 0))
        return -1;

    return 0;
}
