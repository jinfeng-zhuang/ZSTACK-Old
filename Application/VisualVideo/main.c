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
    struct ringbuffer *es;
    int channel = 0;
    float es_percent = 0.0;
    char *version;
    struct VideoFrameInfo* frameinfo;
    triVideoSharedInfo_t* shareinfo;
    int step_cmd;
    unsigned int luma_addr[4];
    
    version = avmips_get_version();
    SendMessage(hwnd, WM_USER, "Version", version);
    
    while (1) {
        es = avmips_get_ves_desc(channel);
        shareinfo = avmips_get_shareinfo(channel);
        frameinfo = avmips_get_frameinfo(channel);
        luma_addr[0] = mpegdisp_luma_addr(channel, 0);
        luma_addr[1] = mpegdisp_luma_addr(channel, 1);
        luma_addr[2] = mpegdisp_luma_addr(channel, 2);
        luma_addr[3] = mpegdisp_luma_addr(channel, 3);
        
        SendMessage(hwnd, WM_USER, "ESRING", es);
        SendMessage(hwnd, WM_USER, "ShareInfo", shareinfo);

        SendMessage(hwnd, WM_USER, "DECODER", shareinfo);
        SendMessage(hwnd, WM_USER, "AVSYNC", shareinfo);
        
        SendMessage(hwnd, WM_USER, "FrameBuffer", luma_addr);
        SendMessage(hwnd, WM_USER, "MPEGFormat", frameinfo);
    
        step_cmd = avmips_get_step_flag();
        if (-1 != step_cmd) {
            avmips_step(step_cmd);
        }

        msleep(100);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    MSG msg;
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    InitCommonControls();

    Class_AVMIPSVideo_Register(NULL);
    Class_AVMIPSVideo_MpegFormat_Register(NULL);
    Class_AVMIPSVideo_Decoder_Register(NULL);
    Class_AVMIPSVideo_Input_Register(NULL);
    Class_AVMIPSVideo_AVSync_Register(NULL);
    Class_AVMIPSVideo_FrameBuffer_Register(NULL);
    Class_YUV_Register(NULL);
    Class_RingBufferRegister(NULL);

    ret = dbg_init(app.param.ip, app.param.timeout);
    if (0 != ret) {
        return -1;
    }
    
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

    thread_create(thread_entry, 0);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
