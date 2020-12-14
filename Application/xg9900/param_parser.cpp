#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,

	// FOR TEST
	OPTION_FREQ,
	OPTION_SAMPLE_FREQ
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
	{"freq", required_argument, 0, OPTION_FREQ},
	{"sample", required_argument, 0, OPTION_SAMPLE_FREQ},
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
		case OPTION_FREQ:
			app->param.freq = atoi(optarg);
			break;
		case OPTION_SAMPLE_FREQ:
			app->param.sample_freq = atoi(optarg);
			break;
        default:
            return -1;
        }
    }

    if (optind >= argc) {
        // argv[optind]
    }

    // Do param validation

    return 0;
}
