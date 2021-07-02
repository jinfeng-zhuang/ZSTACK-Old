#include <zstack.h>

#include "main.h"

extern void sps_dump(struct sps *sps);
extern struct nalu * nalu_get(int index);
extern struct slice_layer_without_partitioning_rbsp *slice_layer_without_partitioning_rbsp_parse(unsigned char* buffer, unsigned int length);

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    struct nalu *nalu;
    struct sps *sps;
    int nalu_count;
    int i;
    struct slice_layer_without_partitioning_rbsp *slice;
    int slice_count;
    int dump_flag;
    int pps_count;
    int iframe_count;
    int pframe_count;

    sps = NULL;
    slice_count = 0;
    pps_count = 0;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    app.input_file_content = file_load(app.param.input_filename, &app.input_file_size);
    if (NULL == app.input_file_content)
        return -1;

    ret = nalu_parse_all(app.input_file_content, app.input_file_size);
    if (-1 == ret)
        return -1;

    nalu_count = ret;

    for (i = 0; i < nalu_count; i++) {
        nalu = nalu_get(i);

        dump_flag = 0;

        if (nalu->type == NALU_SPS) {
            sps = sps_proc(nalu->buffer, nalu->length);
            dump_flag = 1;
        }

        if (nalu->type == NALU_PPS) {
            if (pps_count == 0)
                dump_flag = 1;
            else
                dump_flag = 0;
            pps_count++;
        }

        if (sps && (nalu->type == NALU_SLICE) || ((nalu->type == NALU_SLICE_IDR))) {
            slice = slice_layer_without_partitioning_rbsp_parse(nalu->buffer, nalu->length);
#if 0
            info("[%d][POC %d] frame_mbs_only_flag = %d, mb_adaptive_frame_field_flag = %d, field_pic_flag = %d\n",
                    slice_count++,
                    slice->header.pic_order_cnt_lsb,
                    sps->frame_mbs_only_flag,
                    sps->mb_adaptive_frame_field_flag,
                    slice->header.field_pic_flag);
#endif

            dump_flag = 1;
            slice_count++;
        }

        if (dump_flag) {
        //if (0) {
            file_append(app.param.output_filename,
                nalu->buffer - nalu->prefix - 1,
                nalu->length + nalu->prefix + 1);
        }

        if (slice_count == 1)
            break;
    }

    info("extract to %s\n", app.param.output_filename);

    free(app.input_file_content);

    return 0;
}
