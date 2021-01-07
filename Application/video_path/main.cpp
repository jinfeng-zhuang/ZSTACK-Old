#define LOG_MODULE LOG_MODULE_APP

#include <zstack.h>
#include <vs.h>
#include <Windows.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    struct ring ves;
    unsigned int id;
    unsigned int value;
    unsigned int frameinfo_addr;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    if (app.param.ves_desc_addr != 0)
        vs_ves_descriptor_address = app.param.ves_desc_addr;

    id = vs_chip_id_get();
    if (0 == id) {
        TRACE;
        return -1;
    }

    if (((id) != CHIP_SX7B) && ((id) != CHIP_SX7A) && ((id) != CHIP_SX8B) && ((id) != CHIP_SX8A)) {
        TRACE;
        return -1;
    }

    if (((id) == CHIP_SX7B) || ((id) == CHIP_SX7A)) {
        frameinfo_addr = MPEGDISP_CH1_C0_FRMINFO_SX7;
    }
    else {
        frameinfo_addr = MPEGDISP_CH1_C0_FRMINFO_SX8;
    }

    // STEP 1: Disable AVSYNC
    if (app.param.avsync_disable) {
        ret = vs_disable_avsync();
        if (0 != ret) {
            log_err("Disable AVSYNC Failed\n");
            return -1;
        }

        info("AVSYNC disabled\n");
    }

    do {
        ret = avmips_get_ves_desc(&ves, app.param.channel);
    } while (ret != 0);

    while (1) {
        // STEP 2: ES BUFFER
        ret = avmips_get_ves_desc(&ves, app.param.channel);
        if (0 != ret) {
            warn("try get ves desc...\n");
            continue;
        }

        // STEP 3: Ready2DispQ
        ret = vs_get_Ready2DispQ_value(&value, app.param.channel);
        if (0 != ret) {
            warn("try get frameQ desc...\n");
            continue;
        }

        // STEP 4: MPEGDISP
        log_info("ES Buffer: %2dM %4dK %4dB, Ready2dispQ: %d\n",
            (ringbuf_get_datalen(&ves) >> 20) & 0x3FF,
            (ringbuf_get_datalen(&ves) >> 10) && 0x3FF,
            (ringbuf_get_datalen(&ves)) && 0x3FF,
            value);

        Sleep(10);
    }

    return 0;
}
