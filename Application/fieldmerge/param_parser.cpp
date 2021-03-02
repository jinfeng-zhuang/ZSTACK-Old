#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,
    OPTION_TOP_FILE,
    OPTION_TOP_INDEX,
    OPTION_BOTTOM_FILE,
    OPTION_BOTTOM_INDEX,
    OPTION_RESOLUTION,
    OPTION_TOP_FIELD_FIRST,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
    {"top", required_argument, 0, OPTION_TOP_FILE},
    {"bottom", required_argument, 0, OPTION_BOTTOM_FILE},
    {"top_index", required_argument, 0, OPTION_TOP_INDEX},
    {"bottom_index", required_argument, 0, OPTION_BOTTOM_INDEX},
    {"resolution", required_argument, 0, OPTION_RESOLUTION},
    {"top_field_first", no_argument, 0, OPTION_TOP_FIELD_FIRST},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;
    
#ifdef _WIN32
    SYSTEMTIME time;
#else
    time_t seconds;
    struct tm *tm;
#endif

    if (argc <= 1)
        return -1;

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            log_info("Version: %s\n", version);
            return -1;
        case OPTION_HELP:
            log_info(usage);
            return -1;
        case OPTION_LOG:
            if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
                log_info("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
                return -1;
            }
            strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
            break;
        case OPTION_TOP_FILE:
            if (strlen(optarg) >= FILENAME_MAX)
                return -1;
            memcpy(app->param.top_filename, optarg, strlen(optarg));
            break;
        case OPTION_BOTTOM_FILE:
            if (strlen(optarg) >= FILENAME_MAX)
                return -1;
            memcpy(app->param.bottom_filename, optarg, strlen(optarg));
            break;
        case OPTION_TOP_INDEX:
            app->param.top_index = atoi(optarg);
            break;
        case OPTION_BOTTOM_INDEX:
            app->param.bottom_index = atoi(optarg);
            break;
        case OPTION_RESOLUTION:
            sscanf(optarg, "%dx%d", &app->param.width, &app->param.height);
            break;
        case OPTION_TOP_FIELD_FIRST:
            app->param.top_field_first = 1;
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

    // Do param validation
    if (0 == app->param.output_filename[0]) {
#ifdef _WIN32
        GetLocalTime(&time);
        _snprintf(app->param.output_filename, FILENAME_MAX, "template_%d-%02d-%02d_%02d-%02d-%02d.log",
            time.wYear,
            time.wMonth,
            time.wDay,
            time.wHour,
            time.wMinute,
            time.wSecond
            );
#else
        seconds = time(NULL);
        tm = localtime(&seconds);
        snprintf(app->param.output_filename, FILENAME_MAX, "template_%d-%02d-%02d_%02d-%02d-%02d.log",
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_day,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec
            );
#endif
    }

    return 0;
}
