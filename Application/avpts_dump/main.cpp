#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);
    if (NULL == app.input_file_content)
        return -1;

    unsigned long long *pts_buffer = (unsigned long long *)app.input_file_content;

    int i;

    //info("sizeof VideoPTSQueue_t = %d\n", sizeof(struct VideoPTSQueue_t));

    struct VideoPTSQueue_t *PTS = (struct VideoPTSQueue_t *)app.input_file_content;

    info("WP %x RP %x SIZE %x\n", PTS->wrIdx, PTS->rdIdx, PTS->sz);

    unsigned long long PTSPrev = 0;

    for (i = 0; i < VPTS_QUEUE_SIZE; i++) {
        info("PTS %08llx delta %lld, WP %x %x, delta %x\n",
            PTS->input_pts_map[i], PTS->input_pts_map[i] - PTSPrev,
            PTS->start_wp[i][0], PTS->start_wp[i][1], PTS->start_wp[i][1] - PTS->start_wp[i][0]);

        PTSPrev = PTS->input_pts_map[i];
    }

    return 0;
}
