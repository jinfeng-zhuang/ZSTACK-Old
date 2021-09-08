#include <Windows.h>
#include <commctrl.h>

#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

#include "main.h"

struct application app;

static HWND hwnd;

static int thread_entry(void* arg)
{
    unsigned int es_desc_base = 0;
    unsigned int frameq_base = 0;
    unsigned int rdy2dispQ_count = 0;
    char tmpbuf[100];
    struct ringbuffer r;
    float es_percent = 0.0;

    while (1) {
        if (0 == hwnd)
            goto SLEEP;

#if 0
        if (0 == es_desc_base) {
            es_desc_base = avmips_get_ves_desc_addr(0);
            es_percent = 0.0;
        }
        else {
            avmips_get_ves_desc(&r, es_desc_base);
            es_percent = ringbuf_datalen_percent(&r);
        }
        
        if (0 == frameq_base) {
            frameq_base = avmips_frameq_get_base(0);
            rdy2dispQ_count = 0;
        }
        else {
            avmips_get_ves_desc(&r, frameq_base);
            rdy2dispQ_count = avmips_frameq_get_rdy2dispQ_count(&r);
        }
#else
        es_percent += 0.1;
#endif

        snprintf(tmpbuf, sizeof(tmpbuf), "ES:\n\t%f", es_percent);
        SendMessage(hwnd, WM_USER, 0, tmpbuf);

        SendMessage(hwnd, WM_USER, 1, "SigmaIP");

        snprintf(tmpbuf, sizeof(tmpbuf), "%d", rdy2dispQ_count);
        SendMessage(hwnd, WM_USER, 2, tmpbuf);

        SendMessage(hwnd, WM_USER, 3, "frameinfo:");
SLEEP:
        msleep(1000);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    MSG msg;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    // Load common control DLL
    InitCommonControls();

    Class_AVMIPSVideo_Register(NULL);
    Class_AVMIPSVideo_MpegFormat_Register(NULL);
    Class_AVMIPSVideo_Decoder_Register(NULL);
    Class_YUV_Register(NULL);

    //dbg_init("10.86.79.148");

    //thread_create(thread_entry, 0);

    hwnd = CreateWindowEx(
        0,
        TEXT("AVMIPSVideo"),
        TEXT("VisualVideo"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 800, 600,
        NULL,
        NULL,
        NULL,
        NULL);

    ShowWindow(hwnd, 1);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
