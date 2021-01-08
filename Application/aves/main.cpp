#include <zstack.h>
#include <vs.h>
#include <Windows.h>
#include "main.h"

#define LOG_MODULE LOG_MODULE_APP

#define DUMP_SIZE   (1<<20)
#define SNAPSHOT_BUFFER_SIZE    (12<<20)

struct application app;

static unsigned char buffer[DUMP_SIZE];
static unsigned char snapshot_buffer[SNAPSHOT_BUFFER_SIZE];

int main(int argc, char *argv[])
{
    int ret;
    struct ring ves;
    unsigned int rp;

    unsigned int i;
    
    struct ring ves_local;
    unsigned int size;
    unsigned int response;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    do {
        ret = avmips_get_ves_desc(&ves, app.param.channel);
    } while (ret != 0);

    size = ves.end - ves.start;

    log_info("VES (%x %x), size %dM\n", ves.start, ves.end, size>>20);

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), PMAN_SECURITY_GROUP_ARM, ves.start);
    if (0 != ret) {
        return -1;
    }

    rp = ves.start;

    if (app.param.snapshot) {
        info("snapshot start\n");

        for (i = 0; i < 12; i++) {
            info("%d ", i);

            // it seems read very slow: 5sec/1MB
            ret = dbg_host_read8(ves.start, snapshot_buffer, 1<<20);
            if (0 != ret) {
                log_err("dbg_host_read8 failed\n");
                return -1;
            }

            ret = file_append(app.param.filename, snapshot_buffer, 1<<20);
            if (0 != ret) {
                log_err("file_append failed\n");
                return -1;
            }
        }
        info("snapshot complete\n");

        goto END;
    }

    while (1) {
        ret = avmips_get_ves_desc(&ves, app.param.channel);
        if (0 != ret) {
            return -1;
        }

        memcpy(&ves_local, &ves, sizeof(struct ring));
        ves_local.rp = (rp - ves_local.start) % size + ves_local.start;

        if (app.param.dump_flag) {
            // print remaining data
            log_info("Decoder: %dM %4dK %4dB, Dump: %dM %4dK %4dB\n",
                (ringbuf_get_datalen(&ves) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&ves) >> 10) & 0x3FF,
                ringbuf_get_datalen(&ves) & 0x3FF,

                (ringbuf_get_datalen(&ves_local) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&ves_local) >> 10) & 0x3FF,
                ringbuf_get_datalen(&ves_local) & 0x3FF
                );

            // get start address and size for current ringbuf
            response = ringbuf_measure(&ves_local, DUMP_SIZE);
            if (0 == response) {
                Sleep(100);
                continue;
            }

            ret = dbg_host_read8(ves_local.rp, buffer, response);
            if (0 != ret) {
                return -1;
            }

            ret = file_append(app.param.filename, buffer, response);
            if (0 != ret) {
                return -1;
            }

            rp += response;
        }
        else {
            log_info("Decoder: %dM %4dK %4dB\n",
                (ringbuf_get_datalen(&ves) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&ves) >> 10) & 0x3FF,
                ringbuf_get_datalen(&ves) & 0x3FF);
        }

        Sleep(10);
    }

END:

    return 0;
}
