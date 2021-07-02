#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

static FrameQArray_t frameQ = { 0 };

int vs_get_Ready2DispQ_value(unsigned int *value, unsigned int channel)
{
    int ret;
    unsigned int result;

    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_FRAMEQ | (channel<<6), &result, 1);
    if (0 != ret) {
        warn("can't get address\n");
        return -1;
    }

    ret = dbg_host_read8(result, (unsigned char *)&frameQ, sizeof(frameQ));
    if (0 != ret) {
        warn("dbg_host_read8 failed\n");
        return -1;
    }

    *value = frameQ.rdy2DispQ.wrIdx - frameQ.rdy2DispQ.rdIdx;

    info("frameQ.wrIdx = %d\n", frameQ.rdy2DispQ.wrIdx);
    
    return 0;
}
