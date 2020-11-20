#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <zstack/log.h>
#include <zstack/misc.h>
#include <zstack/yuv.h>
#include <zstack/platform.h>

#include <3rdparty/getopt.h>

const char *usage = 
"fieldmerge - build @ %s\n"
"usage: fieldmerge [options]... <output>\n"
"options:\n"
"\t-d dump <n>                      \n"
"\t-r resolution <720x288>          Field Resolution\n"
"\t-t top <filename:index>          \n"
"\t-w window                        \n"
"\t-b bottom <filename:index>       \n";

void print_usage(void)
{
    printf(usage, __DATE__);
}

struct _param {
    int width;
    int height;
    int top_field_first;
    char top_filename[128];
    char bottom_filename[128];
    char output_filename[128];
    int top_index;
    int bottom_index;
    int func;
    FILE *top_fd;
    FILE *bottom_fd;
    FILE *output_fd;
    unsigned char *top_buffer;
    unsigned char *bottom_buffer;
    unsigned char *output_buffer;

    int *match_list;
};

struct db {
    int top;
    int bottom;
    int match;
    unsigned char *merged_buffer;
};

struct _param param;

static unsigned char *do_merge(int top, int bottom)
{
    int offset_t, offset_b;
    int top_field;
    int field_size = param.width * param.height * 3 / 2;
    int i;

    fseek(param.top_fd, field_size * top, SEEK_SET);
    fread(param.top_buffer, 1, field_size, param.top_fd);

    fseek(param.bottom_fd, field_size * bottom, SEEK_SET);
    fread(param.bottom_buffer, 1, field_size, param.bottom_fd);

    memset(param.output_buffer, 0, field_size * 2);

    if (param.top_field_first) {
        offset_t = 0;
        offset_b = param.width;
    }
    else {
        offset_t = param.width;
        offset_b = 0;
    }

    for (i=0; i<param.height; i++) {
        memcpy(&param.output_buffer[param.width * i * 2 + offset_t], &param.top_buffer[param.width * i], param.width);
        memcpy(&param.output_buffer[param.width * i * 2 + offset_b], &param.bottom_buffer[param.width * i], param.width);
    }

    return param.output_buffer;
}

static YUVWindowStruct r;

static int event_handler(int evt, void *in, void *out)
{
    struct db db;

    memcpy(&db, in, sizeof(struct db));

    if (db.match == 0) {
        r.buffer = do_merge(db.top, db.bottom);
        r.width = param.width;
        r.height = param.height * 2;

        *(unsigned int *)out  = (unsigned int)&r;
    }
    else {
        if (param.match_list) {
            param.match_list[db.top] = db.bottom;
        }
    }

    return 0;
}

static struct db db;

int main(int argc, char *argv[])
{
    int opt;
    unsigned int field_size;
    int flag_window = 0;
    int frame_count;

    log_init(NULL);

    //--------------------------------------------------------------------------------------------

    memset(&param, 0, sizeof(struct param));

    param.top_field_first = 1;

    while ((opt = getopt(argc, argv, "wr:b:t:v")) != -1)  {
        switch (opt) {
            case 'r':
                sscanf(optarg, "%dx%d", &param.width, &param.height);
                break;
            case 'b':
                sscanf(optarg, "%[^:]:%d", &param.bottom_filename, &param.bottom_index);
                break;
            case 't':
                sscanf(optarg, "%[^:]:%d", &param.top_filename, &param.top_index);
                break;
            case 'v':
                param.top_field_first = 0;
                break;
            case 'w':
                flag_window = 1;
                break;
            default:
                log(LOG_WARNING, "Unknown option: %c\n", opt);
                return -1;
        }
    }

    if (argv[optind])
        strcpy(param.output_filename, argv[optind]);

    if ((param.width == 0)||(param.height==0)||(param.bottom_filename[0]==0)||(param.top_filename[0]==0)||(0 == param.output_filename[0])) {
        print_usage();
        return 0;
    }

    //--------------------------------------------------------------------------------------------

    log_init(NULL);

    param.top_fd = fopen(param.top_filename, "rb");
    param.bottom_fd = fopen(param.bottom_filename, "rb");
    param.output_fd = fopen(param.output_filename, "wb");

    if ((NULL == param.top_fd) || (NULL == param.output_fd) || (NULL == param.bottom_fd)) {
        log(LOG_ERROR, "file open failed\n");
        goto FAILED;
    }

    field_size = param.width * param.height * 3 / 2;

    param.top_buffer = (unsigned char *)malloc(field_size);
    param.bottom_buffer = (unsigned char *)malloc(field_size);
    param.output_buffer = (unsigned char *)malloc(field_size * 2);

    if ((NULL == param.top_buffer) || (NULL == param.bottom_buffer) || (NULL == param.output_buffer)) {
        log(LOG_ERROR, "buffer malloc failed\n");
        goto FAILED;
    }

    fseek(param.top_fd, 0, SEEK_END);
    frame_count = ftell(param.top_fd) / field_size;

    log(LOG_USER, "frame count = %d\n", frame_count);

    param.match_list = (int *)malloc(sizeof(int) * frame_count);
    if (NULL == param.match_list)
        goto FAILED;

    //--------------------------------------------------------------------------------------------

    if (flag_window) {
        Platform_Init("FieldMergeWindow", event_handler);
    }
    else {
        db.top = param.top_index;
        db.bottom = param.bottom_index;
        
        //event_handler(MSG_SET_DATA, &db, NULL);

        //fwrite(...);
    }
    
    return 0;

FAILED:
    return -1;
}
