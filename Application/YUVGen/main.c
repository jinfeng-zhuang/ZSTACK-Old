#include <zstack/zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    u32 i;
    u32 frame_num;
    u32 frame_size;
    u8* frame_buffer;
    struct canvas canvas;
    struct rect rect;
    s32 step;

    frame_buffer = NULL;

    if (param_parser(argc, argv, &app) == -1) {
        return -1;
    }

    log_init(app.param.log_config);

    frame_num = app.param.duration * app.param.framerate;

    frame_size = 0;
    if (app.param.format == NV12)
        frame_size = app.param.stride * app.param.height * 3 / 2;
    else
        goto END;

    frame_buffer = (u8 *)malloc(frame_size);
    if (NULL == frame_buffer)
        goto END;

    memset(frame_buffer, 0, frame_size);

    canvas.buffer = frame_buffer;
    canvas.length = frame_size;
    canvas.w = app.param.width;
    canvas.h = app.param.height;
    canvas.stride = app.param.stride;
    canvas.type = NV12;

    rect.x = 0;
    rect.y = 0;
    rect.w = canvas.w / 10;
    rect.h = canvas.h / 10;
    if (rect.w == 0)
        rect.w = 1;
    if (rect.h == 0)
        rect.h = 1;

    step = min(rect.w, rect.h);
    step = 1;

    for (i = 0; i < frame_num; i++) {
        fill_canvas(&canvas, 0xFFFFFFFF);
        fill_rectangle(&canvas, &rect, 0xFF0000FF);

        file_append(app.param.output_filename, canvas.buffer, canvas.length);

        rect.x += step;
        rect.y += step;

        if ((rect.x >= canvas.w) && (rect.y >= canvas.h)) {
            rect.x = 0;
            rect.y = 0;
        }

        //break;
    }

    // TODO
    if (frame_buffer)
        free(frame_buffer);

    return 0;

END:
    if (frame_buffer)
        free(frame_buffer);

    return -1;
}
