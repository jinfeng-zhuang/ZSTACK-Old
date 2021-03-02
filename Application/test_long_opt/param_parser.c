#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

enum {
    OPTION_VERSION,
    OPTION_HELP,
    OPTION_DEBUG,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"debug", no_argument, 0, OPTION_DEBUG},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            printf("version 0.1\n");
            break;
        case OPTION_HELP:
            printf("help\n");
            break;
        }
    }

    return 0;
}
