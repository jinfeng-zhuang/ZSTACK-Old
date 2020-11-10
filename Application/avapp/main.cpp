#define _CRT_SECURE_NO_WARNINGS
#define ENABLE_LOG

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#include <3rdparty/getopt.h>

#include <zstack/net.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include <zstack/ringbuf.h>

#include <vs/dbg.h>
#include <vs/sx.h>
#include <vs/avmips.h>

extern void RingBufferWindow_Init(void);
extern void RingBufferWindow_Set(struct ring *r);

int main(int argc, char* argv[])
{
    int ret;
    int opt;
    unsigned int id;
    char *ip;
    unsigned int ves_addr;
    int flag_loop = 0;
    int flag_dump = 0;
    struct HWDemuxVESDesc_t HWDesc;
    struct AVStreamVESDesc_t Desc;
    int flag_dir = 0;
    int flag_ringbuffer_window = 0;
    unsigned int remain;
    char *debug_config = NULL;

    if (argc < 2) {
        print_usage();
        return -1;
    }

    while ((opt = getopt(argc, argv, "swld:")) != -1)  {
        switch (opt) {
            case 'l':
                flag_loop = 1;
                break;
            case 's':
                break;
            case 'w':
                flag_ringbuffer_window = 1;
                break;
            case 'd':
                debug_config = optarg;
                break;
            default:
                log(LOG_WARNING, "Unknown option: %c\n", opt);
                print_usage();
                return -1;
        }
    }

    ip = argv[optind];

    log_init(debug_config);
    
    ret = dbg_init(ip);
    if (0 == ret) {

        // Get Chip ID
        id = vs_chip_id_get();

        if ((id != CHIP_SX7B) && (id != CHIP_SX7A) && (id != CHIP_SX8B) && (id != CHIP_SX8A)) {
            log(LOG_WARNING, "CHIP Not supported\n");
            goto END;
        }

        if (flag_ringbuffer_window) {
            RingBufferWindow_Init();
        }

        while (1) {
            struct ring r;
            ret = avmips_get_ves_desc(&r);
            if ((ret == 0) && flag_ringbuffer_window) {
                RingBufferWindow_Set(&r);
            }

            if (! flag_loop) {
                break;
            }

            Sleep(1000/100);
        }

        // DISABLE PMAN
    }

END:

    dbg_deinit();

    if (flag_ringbuffer_window) {
        while (1)
            Sleep(1000);
    }

    return 0;
}
