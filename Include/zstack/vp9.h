#ifndef ZSTACK_VP9_H
#define ZSTACK_VP9_H

enum {
    VP9_SEI_PIC_TIMING = 1,
};

struct ivf_file_header {
    unsigned int signature;
    unsigned short version;
    unsigned short length;
    unsigned int codec;
    unsigned short width;
    unsigned short height;
    unsigned int framerate;
    unsigned int timescale;
    unsigned int framenum;
    unsigned int unused;
};

#endif