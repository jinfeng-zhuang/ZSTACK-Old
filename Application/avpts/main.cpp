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

    avmips_get_version();

    if (app.param.analyze_flag) {
        app.input_file_content = file_load(app.param.input_filename, (uint64_t *)&app.input_file_size);
        if (NULL == app.input_file_content)
            goto END;

        int i;
        struct VideoPTSQueue_t* PTS = (struct VideoPTSQueue_t*)app.input_file_content;
        info("WP %x RP %x SIZE %x\n", PTS->wrIdx, PTS->rdIdx, PTS->sz);
        unsigned long long PTSPrev = 0;
        for (i = 0; i < VPTS_QUEUE_SIZE; i++) {
            info("PTS %08llx delta %lld, WP %x %x, delta %#x\n",
                PTS->input_pts_map[i], PTS->input_pts_map[i] - PTSPrev,
                PTS->start_wp[i][0], PTS->start_wp[i][1], PTS->start_wp[i][1] - PTS->start_wp[i][0]);

            PTSPrev = PTS->input_pts_map[i];
        }

        goto END;
    }

    do {
        ret = avmips_get_pts_desc(&pts, app.param.channel);
    } while (ret != 0);

    log_info("PTS (%#x %#x), size %dKB\n", pts.start, pts.end);

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), PMAN_SECURITY_GROUP_ARM, pts.start);
    if (0 != ret) {
        return -1;
    }

    rp = pts.start;

    while (1) {
        do {
            ret = avmips_get_pts_desc(&pts, app.param.channel);
        } while (ret != 0);

        if ((pts.wp >= app.param.dump_count) && (app.param.dump_flag)) {
            buffer = avmips_dump_pts_queue(app.param.channel);
                if (NULL == buffer) {
                    TRACE;
                        goto END;
                }

            ret = file_append(app.param.output_filename, buffer, sizeof(struct VideoPTSQueue_t));
                if (0 != ret) {
                    goto END;
                }

            info("%s\n", app.param.output_filename);

            goto END;
        }

        log_info("Decoder: %dM %4dK %4dB, %f\n",
            (ringbuf_get_datalen(&pts) >> 20) & 0x3FF,
            (ringbuf_get_datalen(&pts) >> 10) & 0x3FF,
            ringbuf_get_datalen(&pts) & 0x3FF,
            (float)pts.wp / (float)pts.end);

        Sleep(10);
    }

END:
    return 0;
}
