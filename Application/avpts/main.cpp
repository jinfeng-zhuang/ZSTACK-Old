//#define LOG_MODULE LOG_MODULE_APP

#include <zstack.h>
#include <vs.h>
#include <Windows.h>
#include "main.h"

#define DUMP_SIZE   (1<<20)
#define SNAPSHOT_BUFFER_SIZE    (12<<20)

struct application app;

static unsigned char buffer[DUMP_SIZE];
static unsigned char snapshot_buffer[SNAPSHOT_BUFFER_SIZE];

int main(int argc, char *argv[])
{
    int ret;
    struct ring pts;
    unsigned int rp;

    unsigned int i;
    
    struct ring pts_local;
    unsigned int size;
    unsigned int response;

    unsigned char *buffer;

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
        ret = avmips_get_pts_desc(&pts, app.param.channel);
    } while (ret != 0);

    size = pts.end - pts.start;

    log_info("PTS (%x %x), size %dM\n", pts.start, pts.end, size>>20);

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), PMAN_SECURITY_GROUP_ARM, pts.start);
    if (0 != ret) {
        return -1;
    }

    rp = pts.start;

    while (1) {
        ret = avmips_get_pts_desc(&pts, app.param.channel);
        if (0 != ret) {
            return -1;
        }

        memcpy(&pts_local, &pts, sizeof(struct ring));
        pts_local.rp = (rp - pts_local.start) % size + pts_local.start;

        if (app.param.dump_flag) {

            /*
            // print remaining data
            log_info("Decoder: %dM %4dK %4dB, Dump: %dM %4dK %4dB\n",
                (ringbuf_get_datalen(&pts) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&pts) >> 10) & 0x3FF,
                ringbuf_get_datalen(&pts) & 0x3FF,

                (ringbuf_get_datalen(&pts_local) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&pts_local) >> 10) & 0x3FF,
                ringbuf_get_datalen(&pts_local) & 0x3FF
                );

            // get start address and size for current ringbuf
            response = ringbuf_measure(&pts_local, DUMP_SIZE);
            if (0 == response) {
                Sleep(100);
                continue;
            }

            ret = dbg_host_read8(pts_local.rp, buffer, response);
            if (0 != ret) {
                return -1;
            }
            */

            buffer = avmips_dump_pts_queue(app.param.channel);
            if (buffer) {
                ret = file_append(app.param.output_filename, buffer, response);
                if (0 != ret) {
                    return -1;
                }
            }

            rp += response;
        }
        else {
            log_info("Decoder: %dM %4dK %4dB\n",
                (ringbuf_get_datalen(&pts) >> 20) & 0x3FF,
                (ringbuf_get_datalen(&pts) >> 10) & 0x3FF,
                ringbuf_get_datalen(&pts) & 0x3FF);
        }

        Sleep(10);
    }

END:

    return 0;
}
