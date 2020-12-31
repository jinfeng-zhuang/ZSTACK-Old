#include <zstack.h>
#include <vs.h>

#include "main.h"

struct application app;

const char *frame_struct_name[] = {
    "TopField",
    "BottomField",
    "Two Field",
    "Frame"
};

const char *codec_name[] = {
    "H264",
    "VC1",
    "MPEG",
    "MPEG4",
    "DIVX3",
    "RV",
    "AVS",
    "JPEG",
    "VP6",
    "SPARK",
    "VP8",
    "M264",
    "MJPEG-A",
    "MJPEG-B",
    "H264",
    "VP9",
    "WMV",
    "MJPEG"
};

const char *output_format_name[] = {
    "Raster Scan Mode",
    "Tile Mode",
    "Compress 8x2",
    "Compress 16x2",
    "Compress 8x4"
};

int main(int argc, char *argv[])
{
    int ret;
    unsigned int id;
    unsigned int cur_frame_info_addr;
    unsigned int bitdepth;
    unsigned int cur_luma_addr;
    unsigned int cur_luma_size;
    unsigned int cur_chroma_addr;
    unsigned int cur_chroma_size;
    unsigned char *luma_data;
    unsigned char *chroma_data;
    struct VideoFrameInfo frame_info;
    struct VideoFrameInfo frame_info_prev;
    struct VideoFrameInfo frame_info_null;
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    char filename[64];
    char *ip;
    unsigned int frameinfo_addr;
    struct mpegdisp_regs regs;
    int disp_width_prev = 0;
    int disp_height_prev = 0;

    int opt = 0;
    int flag_loop = 0;
    int flag_dump = 0;
    int flag_mpeg = 0;
    int flag_profile = 0;
    int flag_edr = 0;
    int flag_hdr = 0;
    int flag_output = 0;
    int flag_frame = 0;
    int flag_simple = 0;
    int flag_regs = 0;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    return 0;
}
