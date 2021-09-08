#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <zstack/zstack.h>
#include <getopt/getopt.h>
#include "main.h"

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,
    OPTION_RESOLUTION,
    OPTION_IFORMAT,
    OPTION_OFORMAT,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
    {"resolution", required_argument, 0, OPTION_RESOLUTION},
    {"iformat", required_argument, 0, OPTION_IFORMAT},
    {"oformat", required_argument, 0, OPTION_OFORMAT},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;
    struct time tm;
    int ret;

    if (argc <= 1)
        return -1;

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            warn("Version: %s\n", version);
            return -1;
        case OPTION_HELP:
            return -1;
        case OPTION_LOG:
            if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
                warn("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
                return -1;
            }
            strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
            break;
        case OPTION_RESOLUTION:
            ret = sscanf(optarg, "%dx%d", &app->param.width, &app->param.height);
            if (2 != ret) {
                printf("resolution format not correct\n");
                return -1;
            }
            break;
        case OPTION_IFORMAT:
            app->param.iformat = YUVFormat_Value(optarg);
            if (YUVFormat_NotSupport == app->param.iformat) {
                printf("format not supported\n");
                return -1;
            }
            break;
        case OPTION_OFORMAT:
            app->param.oformat = YUVFormat_Value(optarg);
            if (YUVFormat_NotSupport == app->param.oformat) {
                printf("format not supported\n");
                return -1;
            }
            break;
        default:
            return -1;
        }
    }

    if (optind < argc) {
        if (strlen(argv[optind]) >= FILENAME_MAX)
            return -1;
        memcpy(app->param.input_filename, argv[optind], strlen(argv[optind]));
    }

    if (optind + 1 < argc) {
        if (strlen(argv[optind + 1]) >= FILENAME_MAX)
            return -1;
        memcpy(app->param.output_filename, argv[optind + 1], strlen(argv[optind + 1]));
    }

    /*************************************************************************
     * Do param validation
     ************************************************************************/

    if (0 == app->param.input_filename[0]) {
        printf("no input file\n");
        return -1;
    }

    if (0 == app->param.output_filename[0]) {
        printf("set default output file\n");
        get_time(&tm);
        snprintf(app->param.output_filename, FILENAME_MAX, "%d-%02d-%02d_%02d-%02d-%02d",
            tm.year,
            tm.month,
            tm.day,
            tm.hour,
            tm.min,
            tm.sec
        );
    }

    return 0;
}
