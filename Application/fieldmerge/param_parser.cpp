#define _CRT_SECURE_NO_WARNINGS

#include "application.h"

int param_parser(int argc, char *argv[], struct application *app)
{
    int opt;

    memset(app, 0, sizeof(struct application));

    app->top_field_first = 1;

    while ((opt = getopt(argc, argv, "wr:b:t:v")) != -1)  {
        switch (opt) {
            case 'r':
                sscanf(optarg, "%dx%d", &app->width, &app->height);
                break;
            case 'b':
                sscanf(optarg, "%[^:]:%d", &app->bottom_filename, &app->bottom_index);
                break;
            case 't':
                sscanf(optarg, "%[^:]:%d", &app->top_filename, &app->top_index);
                break;
            case 'v':
                app->top_field_first = 0;
                break;
            case 'w':
                app->flag_window = 1;
                break;
            default:
                log(LOG_WARNING, "Unknown option: %c\n", opt);
                return -1;
        }
    }

    if (argv[optind])
        strcpy(app->output_filename, argv[optind]);

    if ((app->width == 0)||(app->height==0)||(app->bottom_filename[0]==0)||(app->top_filename[0]==0)||(0 == app->output_filename[0])) {
        goto FAILED;
    }

   app->top_fd = fopen(app->top_filename, "rb");
   app->bottom_fd = fopen(app->bottom_filename, "rb");
   app->output_fd = fopen(app->output_filename, "wb");

    if ((NULL ==app->top_fd) || (NULL ==app->output_fd) || (NULL ==app->bottom_fd)) {
        log(LOG_ERROR, "file open failed\n");
        goto FAILED;
    }

    return 0;

FAILED:
    print_usage();
    return -1;
}
