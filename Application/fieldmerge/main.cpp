#define _CRT_SECURE_NO_WARNINGS

#include "application.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    unsigned int field_size;
    int frame_count;

    log_init(NULL);

    ret = param_parser(argc, argv, &app);
    if (0 != ret)
        return -1;

    field_size = app.width * app.height * 3 / 2;

    app.top_buffer = (unsigned char *)malloc(field_size);
    app.bottom_buffer = (unsigned char *)malloc(field_size);
    app.output_buffer = (unsigned char *)malloc(field_size * 2);

    if ((NULL == app.top_buffer) || (NULL == app.bottom_buffer) || (NULL == app.output_buffer)) {
        log(LOG_ERROR, "buffer malloc failed\n");
        goto FAILED;
    }

    frame_count = fsize(app.top_fd) / field_size;

    log(LOG_USER, "frame count = %d\n", frame_count);

    //--------------------------------------------------------------------------------------------

    if (app.flag_window) {
        //Platform_Init("FieldMergeWindow", event_handler);
    }
    else {
        //db.top = app.top_index;
        //db.bottom = app.bottom_index;
        
        //event_handler(MSG_SET_DATA, &db, NULL);

        //fwrite(...);
    }
    
    return 0;

FAILED:
    return -1;
}
