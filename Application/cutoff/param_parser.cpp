#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,
    OPTION_START,
    OPTION_END,
    OPTION_SIZE,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
    {"start", required_argument, 0, OPTION_START},
    {"end", required_argument, 0, OPTION_END},
    {"size", required_argument, 0, OPTION_SIZE},
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
        case OPTION_START:
            if (optarg && (strlen(optarg) > 2) && (optarg[1] == 'x')) {
                app->param.start = strtol(optarg, NULL, 16);
            }
            else {
                app->param.start = atoi(optarg);
            }
            break;
        case OPTION_END:
            if (optarg && (strlen(optarg) > 2) && (optarg[1] == 'x')) {
                app->param.end = strtol(optarg, NULL, 16);
            }
            else {
                app->param.end = atoi(optarg);
            }
            break;
        case OPTION_SIZE:
            if (optarg && (strlen(optarg) > 2) && (optarg[1] == 'x')) {
                app->param.size = strtol(optarg, NULL, 16);
            }
            else {
                app->param.size = atoi(optarg);
            }
            break;
        default:
            return -1;
        }
    }

    if ((app->param.size == 0) && (app->param.end != 0)) {
        app->param.size = app->param.end - app->param.start;
    }

    if (optind < argc) {
        if (strlen(argv[optind]) >= FILENAME_MAX)
            return -1;
        memcpy(app->param.filename, argv[optind], strlen(argv[optind]));
    }

    if (optind + 1 < argc) {
        if (strlen(argv[optind + 1]) >= FILENAME_MAX)
            return -1;
        memcpy(app->param.output_filename, argv[optind + 1], strlen(argv[optind + 1]));
    }
    else {
        if (app->param.size)
            _snprintf(app->param.output_filename, FILENAME_MAX, "cutoff_%#x_to_%#x.bin", app->param.start, app->param.start + app->param.size);
        else
            _snprintf(app->param.output_filename, FILENAME_MAX, "cutoff_%#x_to_END.bin", app->param.start);
    }

    if (app->param.filename[0] == 0) {
        warn("input file required\n");
        return -1;
    }

    return 0;
}
