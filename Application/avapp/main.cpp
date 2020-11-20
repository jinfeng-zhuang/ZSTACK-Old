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

#define TYPE_ES  0
#define TYPE_PTS 1

int main(int argc, char* argv[])
{
    int ret;
    int opt;
    unsigned int id;
    char *ip;
    unsigned int ves_addr;
    int flag_dump = 0;
    int flag_type = -1;
    int flag_ringbuffer_window = 0;
    unsigned int remain;
    char *debug_config = NULL;

    if (argc < 2) {
        print_usage();
        return -1;
    }

    while ((opt = getopt(argc, argv, "t:swd:")) != -1)  {
        switch (opt) {
            case 's':
                break;
            case 't':
                if (0 == strcmp(optarg, "pts")) {
                    flag_type = TYPE_PTS;
                }
                else if (0 == strcmp(optarg, "es")) {
                    flag_type = TYPE_ES;
                }
                else { // TODO
                    print_usage();
                    return -1;
                }
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

        vs_pman_enable((enum sx_chip)id, PMAN_SECURITY_GROUP_ARM, 0x10c00000);

        if (flag_ringbuffer_window) {
            RingBufferWindow_Init();
        }

        while (1) {
            struct ring r;

            if (flag_type == TYPE_PTS)
                ret = avmips_get_pts_desc(&r);
            else
                ret = avmips_get_ves_desc(&r);

            if ((ret == 0) && flag_ringbuffer_window) {
                RingBufferWindow_Set(&r);
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
