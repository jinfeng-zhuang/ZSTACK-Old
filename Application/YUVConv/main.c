#define _CRT_SECURE_NO_WARNINGS

#include <zstack/zstack.h>
#include <zstack/yuv.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    struct YUV_Wrapper *iwrap;
    struct YUV_Wrapper *owrap;

    size_t bytesread;
    int i, j;
    u16 u, v;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    iwrap = YUV_Wrapper_Init(app.param.iformat);
    owrap = YUV_Wrapper_Init(app.param.oformat);

    if ((NULL == iwrap) || (NULL == owrap)) {
        printf("wrapper init failed: %p %p\n", iwrap, owrap);
        return -1;
    }

    app.iframe_size = iwrap->FrameSize(app.param.width, app.param.height);
    app.oframe_size = owrap->FrameSize(app.param.width, app.param.height);

    app.ifp = fopen(app.param.input_filename, "rb");
    if (NULL == app.ifp) {
        printf("file open failed: %s\n", app.param.input_filename);
        goto END;
    }

    app.iframe = (u8*)malloc(app.iframe_size);
    if (NULL == app.iframe)
        goto END;

    app.oframe = (u8*)malloc(app.oframe_size);
    if (NULL == app.oframe)
        goto END;

    while (1) {
        bytesread = fread(app.iframe, 1, app.iframe_size, app.ifp);
        if (bytesread < app.iframe_size) {
            // EOF, bytesread should be 0.
            if (0 != bytesread) {
                printf("file size not align to the frame size\n");
                goto END;
            }
            else {
                goto END;
            }
        }
        else {
            // copy Y
            memcpy(app.oframe, app.iframe, app.param.width * app.param.height);
            
            // copy u/v
            for (j = 0; j < app.param.height; j++) {
                for (i = 0; i < app.param.width; i++) {
                    u = iwrap->GetU(app.iframe, app.param.width, app.param.height, i, j);
                    v = iwrap->GetV(app.iframe, app.param.width, app.param.height, i, j);
                    owrap->SetU(app.oframe, app.param.width, app.param.height, i, j, u);
                    owrap->SetV(app.oframe, app.param.width, app.param.height, i, j, v);
                }
            }
            
            file_append(app.param.output_filename, app.oframe, app.oframe_size);
        }
    }

END:
    if (app.ifp) {
        fclose(app.ifp);
    }

    if (app.iframe) {
        free(app.iframe);
    }

    if (app.oframe) {
        free(app.oframe);
    }

    return 0;
}
