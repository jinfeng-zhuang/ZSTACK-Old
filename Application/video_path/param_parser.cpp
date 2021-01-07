#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,
    OPTION_VES_DESC_ADDR,
    OPTION_AVSYNC,
    OPTION_CHANNEL,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
    {"ves_desc_addr", required_argument, 0, OPTION_VES_DESC_ADDR},
    {"avsync", required_argument, 0, OPTION_AVSYNC},
    {"channel", required_argument, 0, OPTION_CHANNEL},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;

    if (argc <= 1)
        return -1;

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            log_info("Version: %s\n", version);
            break;
        case OPTION_HELP:
            log_info(usage);
            break;
        case OPTION_LOG:
            if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
                log_info("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
                return -1;
            }
            strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
            break;
        case OPTION_VES_DESC_ADDR:
            app->param.ves_desc_addr = strtol(optarg, NULL, 16);
            break;
        case OPTION_AVSYNC:
            app->param.avsync_disable = atoi(optarg);
            break;
        case OPTION_CHANNEL:
            app->param.channel = atoi(optarg);
            break;
        default:
            return -1;
        }
    }

    if (optind < argc) {
        if (strlen(argv[optind]) >= IP_LENGTH)
            return -1;
        memcpy(app->param.ip, argv[optind], strlen(argv[optind]));
    }

    // Do param validation
    if (app->param.channel > 2)
        return -1;

    return 0;
}
