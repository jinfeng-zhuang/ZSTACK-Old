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
    unsigned int frameinfo_reg;
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

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    if ((vs_chip_id_get() == CHIP_SX7A) || (vs_chip_id_get() == CHIP_SX7B))
        frameinfo_reg = MPEGDISP_CH1_C0_FRMINFO_SX7;
    else
        frameinfo_reg = MPEGDISP_CH1_C0_FRMINFO_SX8;

    while (1) {
        ret = dbg_host_read32(frameinfo_reg | 0xF0000000, &cur_frame_info_addr, 1);
        if (0 == ret) {
            break;
        }
        Sleep(10);
    }

LOOP:
    // 250Byte/10ms => Bandwidth Requirement: 25KB/s ???
    ret = dbg_host_read8(cur_frame_info_addr, (unsigned char *)&frame_info, sizeof(struct VideoFrameInfo));
    if (0 != ret) {
        return -1;
    }

    if (flag_mpeg) {
        info(
            "\tMPEG Format: %dx%d%c @ %s, %s, Stride %d, Display %dx%d\n",
            frame_info.format.wHsize,
            frame_info.format.wVsize,
            frame_info.format.ProgressiveSeq ? 'p' : 'i',
            mpegformat_framerate_name(frame_info.format.FrameRate),
            frame_info.format.PixelDepth ? "10bit" : "8bit",
            frame_info.format.usStride,
            frame_info.format.wDispHsize,
            frame_info.format.wDispVsize
            );
    }

    if (flag_profile) {
        info(
            "\tCodec %s Profile %d Level %d\n",
            codec_name[frame_info.tProfileLvl.VideoStandard],
            frame_info.tProfileLvl.VideoProfile,
            frame_info.tProfileLvl.VideoLevel
            );
    }

    if (flag_hdr) {
        info(
            "\tHDRMode %d Addr 0x%x:\n",
            frame_info.tEDRInfo.HDRModeFlag,
            frame_info.tEDRInfo.DispColorSeiAddr
            );
        hexdump(&frame_info.tDispColor, sizeof(frame_info.tDispColor));
    }

    if (flag_edr) {
        info(
            "\tEDRMode %d Avail %d Addr %x\n",
            frame_info.tEDRInfo.EDRModeFlag,
            frame_info.tEDRInfo.MetaDataAvail,
            frame_info.tEDRInfo.MetaDataAddr
            );
    }

    if (flag_frame) {
        info(
            "\tType '%d' Ref %d Idx %d PTS 0x%08x DecTime %02d(us) TFF %d RFF %d Struct '%s'\n",
            frame_info.tPrivData.PicType,
            frame_info.tPrivData.RefCnt,
            frame_info.tPrivData.FrameIdx,
            frame_info.tPrivData.PTS,
            frame_info.tPrivData.DecTime,
            frame_info.tPrivData.bTopFieldFirst,
            frame_info.tPrivData.bRepeatFirstField,
            frame_struct_name[frame_info.tPrivData.ePictureStructure]
            );
    }

    if (flag_output) {
        if (frame_info.tExtInfo.eOutputFormat < ARRAY_SIZE(output_format_name))
            info(
                "\tOutput format '%s' EDR LUT %x\n",
                output_format_name[frame_info.tExtInfo.eOutputFormat],
                frame_info.tExtInfo.tLUTAddr
                );
        else
            info(
                "\tOutput format %d EDR LUT %x\n",
                frame_info.tExtInfo.eOutputFormat,
                frame_info.tExtInfo.tLUTAddr
                );
    }
    
    if (flag_loop) {
        Sleep(10); // 100Hz > Video_60Hz
        goto LOOP;
    }

    return 0;
}
